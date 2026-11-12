#ifndef __WAVESABRECORE_GSMSAMPLE_H__
#define __WAVESABRECORE_GSMSAMPLE_H__

#include "Helpers.h"

namespace WaveSabreCore
{
	class GsmSample
	{
	public:
		GsmSample(char *data, int compressedSize, int uncompressedSize, void *waveFormat);
		~GsmSample();

		char *WaveFormatData;
		int CompressedSize, UncompressedSize;

		char *CompressedData;
		float *SampleData;

		int SampleLength;

	private:
		static bool driverEnumCallback(void *driverId, unsigned long dwInstance, unsigned long fdwSupport);
		static bool formatEnumCallback(void *driverId, void *formatDetails, unsigned long dwInstance, unsigned long fdwSupport);

		static void *driverId;
	};
}

#endif