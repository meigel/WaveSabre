#ifndef __WAVESABREHEADLESS_OFFLINERENDERER_H__
#define __WAVESABREHEADLESS_OFFLINERENDERER_H__

#include <WaveSabreCore/Device.h>
#include <WaveSabreCore/SynthDevice.h>
#include <WaveSabreIR/Song.h>
#include <vector>
#include <map>
#include <string>

namespace WaveSabreHeadless
{
	class OfflineRenderer
	{
	public:
		OfflineRenderer();
		~OfflineRenderer();

		void LoadSong(const WaveSabreIR::Song& song);
		void Render(float** outputs, int numSamples);

		WaveSabreCore::SynthDevice* GetDevice(const std::string& type,
		                                      const std::map<std::string, float>& parameters);

	private:
		WaveSabreIR::Song song;
		std::map<std::string, WaveSabreCore::SynthDevice*> deviceMap;
		std::vector<WaveSabreCore::SynthDevice*> devices;
		std::vector<std::vector<WaveSabreIR::Note>> scheduledNotes;
		double currentPosition;
	};
}

#endif