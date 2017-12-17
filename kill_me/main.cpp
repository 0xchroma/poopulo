
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

#pragma comment(lib, "C:\\libs\\glfw-3.2.1.bin.WIN64\\lib-vc2015\\glfw3.lib")

#include "types.hpp"
#include "math.hpp"
#include "osc.hpp"
#include "audio_unit.hpp"
#include "audio_renderer.hpp"
#include "envelope.hpp"

using namespace chroma;
using namespace audio;

struct BitchFace : public AudioUnit
{
	Osc oscs[3];
	Envelope env = { 0.5f, 0.5f, 0.5f, 0.5f };

	std::vector<Note> active_notes;

	BitchFace() : AudioUnit("BitchFace")
	{
		oscs[0].type = OscType::Saw;
	}

	void process(Sample* input, Sample* output, size_t sample_count) override
	{
		const auto notes = active_notes;

		for (int i = 0; i < sample_count; i++)
		{
			float signal = 0;

			if (notes.size() > 0)
			{
				float note_freq = note2freq(notes[0]);
				signal = oscs[0].step(1.0f, note_freq);
			}
			else
			{
				env.restart();
			}

			output[i].l = output[i].r = signal;
		}
	}

	void note_on(const Note& note) override
	{
		active_notes.push_back(note);
	}

	void note_off(const Note& note) override
	{
		for (auto iter = active_notes.begin(); iter != active_notes.end(); iter++)
		{
			if (*iter == note)
			{
				active_notes.erase(iter);
				break;
			}
		}
	}

#if _DEBUG
	void gui() override
	{
		ImGui::SliderFloat4("adsr", (float*)&env, 0, 1.0f);
	}
#endif
};

AudioRenderer<44100, 512> renderer;

void CALLBACK MidiInProc(
	HMIDIIN, 
	UINT msg, 
	DWORD_PTR,
	DWORD_PTR p1, 
	DWORD_PTR p2)
{
	if (msg == MIM_DATA)
	{
		char bytes[3] = {};
		auto message = static_cast<DWORD>(p1);
		
		memcpy(bytes, &message, 3);

		switch ((bytes[0] & 0xF0) >> 4)
		{
		case 0x08: // note off
		{
			renderer.channels.front().inst->note_off(static_cast<Note>(bytes[1]));
		} break;
		case 0x09: // note on
		{
			renderer.channels.front().inst->note_on(static_cast<Note>(bytes[1]));
		} break;
		}

		std::cout << std::hex << bytes[1] << std::endl;
		std::cout << note2freq(static_cast<Note>(bytes[1])) << std::endl;
	}
}

int main(int, char**)
{
	GLFWwindow* window = nullptr;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1280, 720, "kill_me", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	ImGui_ImplGlfwGL3_Init(window, true);

	renderer.add_channel(Channel().set_inst(new BitchFace));
	renderer.start();

	HMIDIIN midi_handle = {};
	midiInOpen(&midi_handle, 0, reinterpret_cast<DWORD_PTR>(MidiInProc), 0, CALLBACK_FUNCTION);
	midiInStart(midi_handle);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		int index = 0;
		for (auto& channel : renderer.channels)
		{
			std::string name = "Channel: " + index++;
			ImGui::Begin(name.c_str());
			ImGui::SliderFloat("Vol", &channel.vol, 0.0f, 1.0f);
			ImGui::SliderFloat("Pan", &channel.pan, -1.0f, 1.0f);
			ImGui::Separator();
			ImGui::Text(channel.inst->name);
			channel.inst->gui();
			ImGui::End();
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	midiInStop(midi_handle);
	renderer.stop();

	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}