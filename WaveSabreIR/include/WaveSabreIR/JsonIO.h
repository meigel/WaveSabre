#ifndef __WAVESABREIR_JSONIO_H__
#define __WAVESABREIR_JSONIO_H__

#include "Song.h"
#include <string>

namespace WaveSabreIR
{
	class JsonIO
	{
	public:
		static Song Load(const char *fileName);
		static void Save(const Song& song, const char *fileName);
		static std::string ToJson(const Song& song);
		static Song FromJson(const std::string& json);
	};
}

#endif