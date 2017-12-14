
#include <iostream>
#include <list>

#include <Audioclient.h>
#include <mmdeviceapi.h>

void init_audio()
{
	IAudioClient* audio_client = nullptr;
	IMMDevice* device = nullptr;
	IMMDeviceEnumerator* device_enum = nullptr;
	WAVEFORMATEX* wave_format;
	
	CoInitialize(nullptr);

	CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		nullptr,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&device_enum);

	device_enum->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia, &device);
	device->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&audio_client);
	audio_client->GetMixFormat(&wave_format);
	audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, wave_format, nullptr);


}

void shutdown_audio()
{

}

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
	std::list<Param> params;

	AudioUnit(std::list<Param> _params) : params(_params) { }
	virtual void process(Sample* input, Sample* output, size_t sample_count) = 0;
};

int main(int, char**)
{
	AudioUnit* generator = nullptr;
	std::list<AudioUnit*> effects;

	Sample* samples = new Sample[512];

	generator->process(nullptr, samples, 512);

	for (auto& fx : effects)
	{
		fx->process(samples, samples, 512);
	}

	return 0;
}