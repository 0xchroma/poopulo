
#include <iostream>
#include <list>
#include <chrono>
#include <thread>

#include <portaudio.h>

#pragma comment(lib, "C:/libs/portaudio/build/msvc/x64/Debug/portaudio_x64.lib")

#define SAMPLERATE	44100
#define PI			3.14159265
#define PI2			6.2831853

struct MIDIEvent
{
	char data[4];
	unsigned long timestamp;
};

struct Param
{
	char name[16];
	float value;
};

struct Sample
{
	float l;
	float r;
};

struct AudioUnit
{
	char name[32];
	std::list<Param> params;

	AudioUnit(const char* _name) { memcpy(name, _name, strlen(_name)); }

	virtual void process(Sample* input, Sample* output, size_t sample_count) = 0;

protected:
	void add_param(char* name, float value)
	{
		Param p;

		memcpy(p.name, name, strlen(name));
		p.value = value;

		params.push_back(p);
	}
};

enum class OscType { Sine, Saw, Square, Triangle };
struct Osc
{
	OscType type;
	float phase = 0;

	float step(float amp, float freq)
	{
		float v = 0;

		switch (type)
		{
		case OscType::Sine:
		{
			v = amp * std::sin(phase);
		} break;

		case OscType::Saw:
		{
			v = amp - (amp / PI * phase);
		} break;

		case OscType::Square:
		{
			if (phase < PI)
				v = amp;
			else
				v = -amp;
		} break;

		case OscType::Triangle:
		{
			if (phase < PI)
				v = -amp + (2 * amp / PI) * phase;
			else
				v = amp - (2 * amp / PI) * phase;
		} break;
		}

		phase = phase + ((PI2 * freq) / SAMPLERATE);

		if (phase > (PI2))
			phase = phase - (PI2);

		return v;
	}
};

struct BitchFace : public AudioUnit
{
	Osc oscs[3];

	BitchFace() : AudioUnit("BitchFace")
	{

	}

	void process(Sample* input, Sample* output, size_t sample_count) override
	{
		for (int i = 0; i < sample_count; i++)
		{
			output[i].l = oscs[0].step(0.9f, 440.0f);
		}
	}
};

struct TimeMachine : public AudioUnit
{
	TimeMachine() : AudioUnit("TimeMachine")
	{
	}
};

struct Channel
{
	AudioUnit* inst = nullptr;
	std::list<AudioUnit*> effects;

	float vol = 1.0f;
	float pan = 0.0f;

	void process(Sample* samples, size_t sample_count)
	{
		inst->process(nullptr, samples, sample_count);

		for (auto& fx : effects)
			fx->process(samples, samples, sample_count);
	}
};

struct AudioRenderer;
static int audio_callback(
	const void* input_buffer,
	void* output_buffer,
	unsigned long frame_count,
	const PaStreamCallbackTimeInfo* time_info,
	PaStreamCallbackFlags status_flags,
	void* user_data
)
{
	auto renderer = reinterpret_cast<AudioRenderer*>(user_data);
	auto sample = reinterpret_cast<Sample*>(output_buffer);

	for (int i = 0; i < frame_count; i++)
	{
	}

	return 0;
}

struct AudioRenderer
{
	std::list<Channel> channels;

	PaStream* stream = nullptr;

	AudioRenderer()
	{
		Pa_Initialize();
		Pa_OpenDefaultStream(
			&stream,
			0,
			2,
			paFloat32,
			SAMPLERATE,
			512,
			audio_callback,
			reinterpret_cast<void*>(this));
	}

	~AudioRenderer()
	{
		stop();
		Pa_Terminate();
	}

	void start()
	{
		Pa_StartStream(stream);
	}

	void stop()
	{
		Pa_StopStream(stream);
	}
};

int main(int, char**)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}