#include <OfflineRenderer.h>
#include <WaveSabreCore/Slaughter.h>
#include <WaveSabreCore/Falcon.h>
#include <cmath>
#include <algorithm>
#include <cstring>

namespace WaveSabreHeadless
{
	static const char* slaughterParamNames[] = {
		"Osc1Waveform", "Osc1PulseWidth", "Osc1Volume", "Osc1DetuneCoarse", "Osc1DetuneFine",
		"Osc2Waveform", "Osc2PulseWidth", "Osc2Volume", "Osc2DetuneCoarse", "Osc2DetuneFine",
		"Osc3Waveform", "Osc3PulseWidth", "Osc3Volume", "Osc3DetuneCoarse", "Osc3DetuneFine",
		"NoiseVolume",
		"FilterType", "FilterFreq", "FilterResonance", "FilterModAmt",
		"AmpAttack", "AmpDecay", "AmpSustain", "AmpRelease",
		"ModAttack", "ModDecay", "ModSustain", "ModRelease",
		"MasterLevel",
		"VoicesUnisono", "VoicesDetune", "VoicesPan",
		"VibratoFreq", "VibratoAmount",
		"Rise",
		"PitchAttack", "PitchDecay", "PitchSustain", "PitchRelease", "PitchEnvAmt",
		"VoiceMode", "SlideTime",
	};

	static const char* falconParamNames[] = {
		"Osc1Waveform", "Osc1RatioCoarse", "Osc1RatioFine", "Osc1Feedback", "Osc1FeedForward",
		"Osc1Attack", "Osc1Decay", "Osc1Sustain", "Osc1Release",
		"Osc2Waveform", "Osc2RatioCoarse", "Osc2RatioFine", "Osc2Feedback",
		"Osc2Attack", "Osc2Decay", "Osc2Sustain", "Osc2Release",
		"MasterLevel",
		"VoicesUnisono", "VoicesDetune", "VoicesPan",
		"VibratoFreq", "VibratoAmount",
		"Rise",
		"PitchAttack", "PitchDecay", "PitchSustain", "PitchRelease", "PitchEnvAmt1", "PitchEnvAmt2",
		"VoiceMode", "SlideTime",
	};

	OfflineRenderer::OfflineRenderer()
		: currentPosition(0.0)
	{
	}

	OfflineRenderer::~OfflineRenderer()
	{
		for (auto dev : devices)
			delete dev;
	}

	WaveSabreCore::SynthDevice* OfflineRenderer::GetDevice(const std::string& type,
	                                                        const std::map<std::string, float>& parameters)
	{
		std::string key = type;
		for (const auto& p : parameters)
			key += ":" + p.first + "=" + std::to_string(p.second);

		auto it = deviceMap.find(key);
		if (it != deviceMap.end())
			return it->second;

		WaveSabreCore::SynthDevice* dev = nullptr;
		if (type == "Slaughter")
			dev = new WaveSabreCore::Slaughter();
		else if (type == "Falcon")
			dev = new WaveSabreCore::Falcon();
		else
			return nullptr;

		for (const auto& p : parameters)
		{
			int paramIndex = -1;
			if (type == "Slaughter")
			{
				for (int i = 0; i < (int)WaveSabreCore::Slaughter::ParamIndices::NumParams; i++)
				{
					if (std::string(slaughterParamNames[i]) == p.first)
					{
						paramIndex = i;
						break;
					}
				}
			}
			else if (type == "Falcon")
			{
				for (int i = 0; i < (int)WaveSabreCore::Falcon::ParamIndices::NumParams; i++)
				{
					if (std::string(falconParamNames[i]) == p.first)
					{
						paramIndex = i;
						break;
					}
				}
			}
			if (paramIndex >= 0)
				dev->SetParam(paramIndex, p.second);
		}

		dev->SetSampleRate(44100.0f);
		dev->SetTempo((int)song.tempo.bpm);

		deviceMap[key] = dev;
		devices.push_back(dev);
		return dev;
	}

	void OfflineRenderer::LoadSong(const WaveSabreIR::Song& song_)
	{
		song = song_;
		scheduledNotes.resize(song.tracks.size());
		for (size_t t = 0; t < song.tracks.size(); t++)
		{
			for (const auto& note : song.tracks[t].notes)
			{
				scheduledNotes[t].push_back(note);
			}
		}
		currentPosition = 0.0;
	}

	void OfflineRenderer::Render(float** outputs, int numSamples)
	{
		float* inputBuffers[2] = { nullptr, nullptr };

		for (int s = 0; s < numSamples; s++)
		{
			for (size_t t = 0; t < song.tracks.size(); t++)
			{
				auto dev = GetDevice(song.tracks[t].device.type, song.tracks[t].device.parameters);
				if (!dev) continue;

				auto& notes = scheduledNotes[t];
				for (auto it = notes.begin(); it != notes.end(); )
				{
					if (it->timeSamples <= currentPosition + s)
					{
						int noteNum = it->note;
						int velocity = (int)(it->velocity * 127.0f);
						dev->NoteOn(noteNum, velocity, 0);
						++it;
					}
					else break;
				}
			}

			float* outBuffers[2] = { outputs[0] + s, outputs[1] + s };
			float* inBuffers[2] = { nullptr, nullptr };
			for (auto dev : devices)
				dev->Run(currentPosition, inBuffers, outBuffers, 1);

			for (size_t t = 0; t < song.tracks.size(); t++)
			{
				auto dev = GetDevice(song.tracks[t].device.type, song.tracks[t].device.parameters);
				if (!dev) continue;

				auto& notes = scheduledNotes[t];
				for (auto it = notes.begin(); it != notes.end(); )
				{
					if (it->timeSamples + it->durationSamples <= currentPosition + s)
					{
						int noteNum = it->note;
						dev->NoteOff(noteNum, 0);
						it = notes.erase(it);
					}
					else ++it;
				}
			}

			outputs[0][s] = std::max(-1.0f, std::min(1.0f, outputs[0][s]));
			outputs[1][s] = std::max(-1.0f, std::min(1.0f, outputs[1][s]));
		}
	}
}