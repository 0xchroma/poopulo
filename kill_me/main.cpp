
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

#pragma comment(lib, "C:\\libs\\glfw-3.2.1.bin.WIN64\\lib-vc2015\\glfw3.lib")

#include "types.hpp"
#include "osc.hpp"
#include "audio_unit.hpp"
#include "audio_renderer.hpp"

using namespace chroma;

enum class EnvState { Attack, Decay, Sustain, Release };
struct Envelope
{
	float a;
	float d;
	float s;
	float r;

	const float a_base = 0.3f;
	const float d_min = 0.3f;
	const float s_min = 0.3f;
	const float r_min = 0.3f;

	EnvState state;
	size_t time = 0;
	float v = 0;

	float get()
	{
		switch (state)
		{
		case EnvState::Attack:
		{
			//if (v >= 1.0f)
			//	state = EnvState::Decay;
		} break;
		case EnvState::Decay:
		{
			if (v >= 1.0f)
				state = EnvState::Sustain;
		} break;
		case EnvState::Sustain:
		{
			if (v >= 1.0f)
				state = EnvState::Release;
		} break;
		case EnvState::Release:
		{
		} break;
		}

		time++;

		return v;
	}

private:
	void restart() 
	{ 
		time = 0;
		state = EnvState::Attack;
		v = a;
	}

	float calc_coef(float rate, float target_ratio)
	{
		if (rate <= 0.0)
			return 0.0f;

		return std::exp(-log((1.0f + target_ratio) / target_ratio) / rate);
	}
};

struct Filter
{
	float tap[8];

	float get(float input, float cutoff, float res)
	{
		return 0;
	}
};

struct BitchFace : public AudioUnit
{
	Osc oscs[3];
	Osc lfo = { OscType::Sine };
	Envelope env = { 0.0f, 1.0f, 1.0f, 0.0f };
	Filter filter;

	BitchFace() : AudioUnit("BitchFace")
	{
		oscs[0].type = OscType::Saw;
		oscs[1].type = OscType::Saw;
	}

	void process(Sample* input, Sample* output, size_t sample_count) override
	{
		for (int i = 0; i < sample_count; i++)
		{
			auto amp = env.get();

			auto o1 = oscs[0].step(amp, 220.0f / 3);
			auto o2 = oscs[1].step(amp, 219.0f / 3);

			auto mix = MIX2(o1, o2);

			output[i].l = mix;
			output[i].r = mix;
		}
	}
};

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

	AudioRenderer<44100, 512> renderer;
	renderer.add_channel(Channel().set_inst(new BitchFace));
	renderer.start();
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		ImGui::Begin("fuck");
		ImGui::Text("%f", reinterpret_cast<BitchFace*>(renderer.channels.front().inst)->env.v);
		ImGui::End();

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	renderer.stop();

	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}