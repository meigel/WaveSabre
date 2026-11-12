#ifndef __WAVESABREPLAYERLIB_DIRECTSOUNDRENDERTHREAD_H__
#define __WAVESABREPLAYERLIB_DIRECTSOUNDRENDERTHREAD_H__

#include "SongRenderer.h"

#if defined(_WIN32) && defined(WAVESABRE_BUILD_WIN32_AUDIO)

#include <Windows.h>
#include <dsound.h>

namespace WaveSabrePlayerLib
{
	class DirectSoundRenderThread
	{
	public:
		typedef void (*RenderCallback)(SongRenderer::Sample *buffer, int numSamples, void *data);

		DirectSoundRenderThread(RenderCallback callback, void *callbackData, int sampleRate, int bufferSizeMs = 1000);
		~DirectSoundRenderThread();

		double GetPlayPositionMs();

	private:
		static DWORD WINAPI threadProc(LPVOID lpParameter);

		RenderCallback callback;
		void *callbackData;
		int sampleRate;
		int bufferSizeMs;
		int bufferSizeBytes;

		HANDLE thread;
		CriticalSection criticalSection;
		CriticalSection playPositionCriticalSection;
		bool shutdown;

		int oldPlayCursorPos;
		double bytesRendered;
		LPDIRECTSOUNDBUFFER buffer;
	};
}

#endif

#endif
