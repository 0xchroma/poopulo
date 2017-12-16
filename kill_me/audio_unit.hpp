#pragma once

template<typename T>
struct vector;

namespace chroma
{
	struct Param
	{
		char name[16];
		float value;
	};

	struct AudioUnit
	{
		char name[32];
		std::list<Param> param;

		AudioUnit(const char* _name) { memcpy(name, _name, strlen(_name)); }
		virtual ~AudioUnit() { }

		virtual void process(Sample* input, Sample* output, size_t sample_count) = 0;

		AudioUnit* load(const std::vector<char>& data)
		{
			for (int i = 0; i < data.size(); i += sizeof(Param))
				param.push_back(*reinterpret_cast<const Param*>(data.data() + i));

			return this;
		}

		void save(std::vector<char>& data)
		{
			for (auto& p : param)
				data.insert(data.end(), reinterpret_cast<char*>(&p), reinterpret_cast<char*>(&p + sizeof(Param)));
		}

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