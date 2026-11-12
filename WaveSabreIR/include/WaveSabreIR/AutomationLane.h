#ifndef __WAVESABREIR_AUTOMATIONLANE_H__
#define __WAVESABREIR_AUTOMATIONLANE_H__

#include <string>
#include <vector>

namespace WaveSabreIR
{
	struct AutomationPoint
	{
		int64_t timeSamples = 0;
		float value = 0.0f;
	};

	struct AutomationLane
	{
		std::string target;
		std::string interpolation = "linear";
		std::vector<AutomationPoint> points;
	};
}

#endif