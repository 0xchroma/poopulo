#pragma once

#include <list>
#include <vector>

#include "types.hpp"
#include "audio_unit.hpp"

namespace chroma
{
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

		Channel& set_inst(AudioUnit* unit, const std::vector<char>& preset = {})
		{
			inst = unit;

			if (!preset.empty())
				inst->load(preset);

			return *this;
		}

		Channel& insert_effect(AudioUnit* unit, const std::vector<char>& preset = {})
		{
			if (!preset.empty())
				unit->load(preset);

			effects.push_back(unit);
			return *this;
		}
	};
}