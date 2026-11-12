#ifndef __WAVESABREIR_SONG_H__
#define __WAVESABREIR_SONG_H__

#include <vector>
#include "Track.h"

namespace WaveSabreIR
{
	struct Tempo
	{
		double bpm = 128.0;
	};

	struct Marker
	{
		std::string name;
		int64_t timeSamples = 0;
	};

	struct Song
	{
		int version = 1;
		int sampleRate = 44100;
		Tempo tempo;
		std::vector<Track> tracks;
		std::vector<Marker> markers;
	};
}

#endif