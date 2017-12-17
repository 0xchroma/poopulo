#pragma once

#include <list>
#include "channel.hpp"

#include <portaudio.h>

#pragma comment(lib, "C:/libs/portaudio/build/msvc/x64/Debug/portaudio_x64.lib")

namespace chroma
{
	namespace audio
	{
		static int audio_callback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);

		template<size_t SampleRate, size_t BlockSize>
		struct AudioRenderer
		{
			PaStream* stream = nullptr;
			size_t bpm = 120;
			std::list<Channel> channels;

			AudioRenderer()
			{
				Pa_Initialize();
				Pa_OpenDefaultStream(
					&stream,
					0,
					2,
					paFloat32,
					SampleRate,
					BlockSize,
					audio_callback,
					reinterpret_cast<void*>(this));
			}

			~AudioRenderer()
			{
				stop();
				Pa_Terminate();
			}

			void add_channel(Channel& channel)
			{
				channels.emplace_back(channel);
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

		static int audio_callback(
			const void* input_buffer,
			void* output_buffer,
			unsigned long frame_count,
			const PaStreamCallbackTimeInfo* time_info,
			PaStreamCallbackFlags status_flags,
			void* user_data
		)
		{
			auto renderer = reinterpret_cast<AudioRenderer<44100, 512>*>(user_data);
			auto samples = reinterpret_cast<Sample*>(output_buffer);

			for (auto& channel : renderer->channels)
			{
				channel.inst->process(nullptr, samples, frame_count);
			}

			return 0;
		}
	}
}