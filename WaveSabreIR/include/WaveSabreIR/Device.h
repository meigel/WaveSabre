#ifndef __WAVESABREIR_DEVICE_H__
#define __WAVESABREIR_DEVICE_H__

#include <map>
#include <string>

namespace WaveSabreIR
{
	struct Device
	{
		std::string type;
		std::map<std::string, float> parameters;
	};
}

#endif