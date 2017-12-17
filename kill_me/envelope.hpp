#pragma once

#include <cmath>

namespace chroma
{
	namespace audio
	{
		enum class EnvState { Attack, Decay, Sustain, Release };
		struct Envelope
		{
			float a;
			float d;
			float s;
			float r;

			EnvState state;
			float output = 0;

			void process()
			{
				switch (state)
				{
				case EnvState::Attack:
				{
					output += std::log(std::exp(1.0 - (a - 0.01f))) / 16;

					if (output >= 1.0f)
					{
						output = 1.0f;
						state = EnvState::Decay;
					}
				} break;
				case EnvState::Decay:
				{
					output -= std::log(std::exp(1.0 - (d - 0.01f))) / 16;
					if (output <= s)
					{
						output = s;
						state = EnvState::Sustain;
					}
				} break;
				case EnvState::Sustain:
				{
					state = EnvState::Release;

					/*if (output <= 0)
					{
					output = 0;
					}*/
				} break;
				case EnvState::Release:
				{
					output -= std::log(std::exp(1.0 - (r - 0.01f))) / 16;

					if (output <= 0)
						output = 0;
				} break;
				}
			}

			float out() const
			{
				return output;
			}

			void restart()
			{
				output = 0;
				state = EnvState::Attack;
			}
		};
	}
}