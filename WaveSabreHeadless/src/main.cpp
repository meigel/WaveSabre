#include <OfflineRenderer.h>
#include <WaveSabreIR/JsonIO.h>
#include <iostream>
#include <fstream>

using namespace WaveSabreHeadless;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Usage: wsir-render <input.wsir.json> <output.wav> [duration_seconds]" << std::endl;
		return 1;
	}

	try
	{
		auto song = WaveSabreIR::JsonIO::Load(argv[1]);

		OfflineRenderer renderer;
		renderer.LoadSong(song);

		int sampleRate = song.sampleRate > 0 ? (int)song.sampleRate : 44100;
		int durationSec = (argc >= 4) ? std::atoi(argv[3]) : 10;
		int numSamples = sampleRate * durationSec;

		float* buffers[2];
		buffers[0] = new float[numSamples]();
		buffers[1] = new float[numSamples]();

		renderer.Render(buffers, numSamples);

		int16_t* intBuf = new int16_t[numSamples * 2];
		for (int i = 0; i < numSamples; i++)
		{
			intBuf[i * 2] = static_cast<int16_t>(std::max(-1.0f, std::min(1.0f, buffers[0][i])) * 32767.0f);
			intBuf[i * 2 + 1] = static_cast<int16_t>(std::max(-1.0f, std::min(1.0f, buffers[1][i])) * 32767.0f);
		}

		std::ofstream out(argv[2], std::ios::binary);
		out.write("RIFF", 4);
		int fileSize = 36 + numSamples * 4;
		out.write(reinterpret_cast<char*>(&fileSize), 4);
		out.write("WAVE", 4);
		out.write("fmt ", 4);
		int fmtSize = 16;
		out.write(reinterpret_cast<char*>(&fmtSize), 4);
		int16_t audioFormat = 1;
		out.write(reinterpret_cast<char*>(&audioFormat), 2);
		int16_t numChannels = 2;
		out.write(reinterpret_cast<char*>(&numChannels), 2);
		out.write(reinterpret_cast<char*>(&sampleRate), 4);
		int byteRate = sampleRate * 4;
		out.write(reinterpret_cast<char*>(&byteRate), 4);
		int16_t blockAlign = 4;
		out.write(reinterpret_cast<char*>(&blockAlign), 2);
		int16_t bitsPerSample = 16;
		out.write(reinterpret_cast<char*>(&bitsPerSample), 2);
		out.write("data", 4);
		int dataSize = numSamples * 4;
		out.write(reinterpret_cast<char*>(&dataSize), 4);
		out.write(reinterpret_cast<char*>(intBuf), numSamples * 4);

		delete[] buffers[0];
		delete[] buffers[1];
		delete[] intBuf;

		std::cout << "Rendered " << numSamples << " samples (" << durationSec << "s) to " << argv[2] << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}