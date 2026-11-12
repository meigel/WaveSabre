#ifndef __WAVESABREIR_TRACK_H__
#define __WAVESABREIR_TRACK_H__

#include <string>
#include <vector>
#include "Device.h"
#include "Note.h"
#include "AutomationLane.h"

namespace WaveSabreIR
{
	struct Track
	{
		std::string name;
		Device device;
		std::vector<Note> notes;
		std::vector<AutomationLane> automation;
	};
}

#endif