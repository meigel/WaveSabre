#ifndef __WAVESABREIR_NOTE_H__
#define __WAVESABREIR_NOTE_H__

#include <cstdint>

namespace WaveSabreIR
{
	struct Note
	{
		int64_t timeSamples = 0;
		int64_t durationSamples = 0;
		int note = 60;
		float velocity = 1.0f;
	};
}

#endif