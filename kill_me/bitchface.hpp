#pragma once

#include <vector>

#include "types.hpp"
#include "audio_unit.hpp"
#include "osc.hpp"
#include "envelope.hpp"

#include "imgui\imgui.h"

namespace chroma
{
	namespace audio
	{
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
	}
}