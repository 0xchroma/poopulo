
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
#include "audio_renderer.hpp"
#include "bitchface.hpp"

using namespace chroma;
using namespace audio;

AudioRenderer<44100, 256> renderer;

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