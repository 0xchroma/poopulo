#pragma once

template<typename T>
struct vector;

template<typename T>
struct list;

namespace chroma
{
	namespace audio
	{
		float note2freq(const Note& note)
		{
			return 440.0 * pow(2.0, (static_cast<char>(note) - 69.0) / 12.0);
		}

		struct Param
		{
			char name[8];
			float value;
		};

		struct AudioUnit
		{
			char name[32];
			std::list<Param> param;

			AudioUnit(const char* _name) { memcpy(name, _name, strlen(_name)); name[strlen(_name)] = '\0'; }
			virtual ~AudioUnit() { }

			virtual void process(Sample* input, Sample* output, size_t sample_count) = 0;

			virtual void gui() { }
			virtual void note_on(const Note& note) { }
			virtual void note_off(const Note& note) { }

		protected:
			void add_param(char* name, float value)
			{
				Param p;

				memcpy(p.name, name, strlen(name));
				p.value = value;

				param.push_back(p);
			}
		};
	}
}