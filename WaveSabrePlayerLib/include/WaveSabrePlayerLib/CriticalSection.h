#ifndef __WAVESABREPLAYERLIB_CRITICALSECTION__
#define __WAVESABREPLAYERLIB_CRITICALSECTION__

#if defined(_WIN32) && defined(WAVESABRE_BUILD_WIN32_AUDIO)

#include <Windows.h>

namespace WaveSabrePlayerLib
{
	class CriticalSection
	{
	public:
		class CriticalSectionGuard
		{
		public:
			CriticalSectionGuard(CriticalSection *criticalSection);
			~CriticalSectionGuard();

		private:
			CriticalSection *criticalSection;
		};

		CriticalSection();
		~CriticalSection();

		CriticalSectionGuard Enter();

	private:
		CRITICAL_SECTION criticalSection;
	};
}

#else

namespace WaveSabrePlayerLib
{
	class CriticalSection
	{
	public:
		class CriticalSectionGuard
		{
		public:
			CriticalSectionGuard(CriticalSection *) { }
			~CriticalSectionGuard() { }
		};

		CriticalSection() { }
		~CriticalSection() { }

		CriticalSectionGuard Enter() { return CriticalSectionGuard(this); }
	};
}

#endif

#endif