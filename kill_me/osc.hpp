#pragma once

#define SAMPLERATE	44100
#define PI			3.14159265
#define PI2			6.2831853

#define MIX2(s1, s2) ((s1 + s2) / 2)
#define MIX3(s1, s2, s3) ((s1 + s2 + s3) / 3)
#define MIX4(s1, s2, s3, s4) ((s1 + s2 + s3 + s4) / 4)

namespace chroma
{
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
}