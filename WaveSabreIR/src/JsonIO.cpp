#include <WaveSabreIR/JsonIO.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

namespace WaveSabreIR
{
	Song JsonIO::Load(const char *fileName)
	{
		std::ifstream file(fileName);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return FromJson(buffer.str());
	}

	void JsonIO::Save(const Song& song, const char *fileName)
	{
		std::ofstream file(fileName);
		file << ToJson(song);
	}

	std::string JsonIO::ToJson(const Song& song)
	{
		nlohmann::json j;
		j["version"] = song.version;
		j["sampleRate"] = song.sampleRate;
		j["tempo"]["bpm"] = song.tempo.bpm;
		for (size_t ti = 0; ti < song.tracks.size(); ti++)
		{
			const Track& track = song.tracks[ti];
			nlohmann::json t;
			t["name"] = track.name;
			t["device"]["type"] = track.device.type;
			for (auto pit = track.device.parameters.begin(); pit != track.device.parameters.end(); ++pit)
				t["device"]["parameters"][pit->first] = pit->second;
			nlohmann::json notesArr = nlohmann::json::array();
			for (size_t ni = 0; ni < track.notes.size(); ni++)
			{
				const Note& note = track.notes[ni];
				nlohmann::json n;
				n["timeSamples"] = note.timeSamples;
				n["durationSamples"] = note.durationSamples;
				n["note"] = note.note;
				n["velocity"] = note.velocity;
				notesArr.push_back(n);
			}
			t["notes"] = notesArr;
			nlohmann::json autoArr = nlohmann::json::array();
			for (size_t ai = 0; ai < track.automation.size(); ai++)
			{
				const AutomationLane& lane = track.automation[ai];
				nlohmann::json a;
				a["target"] = lane.target;
				a["interpolation"] = lane.interpolation;
				nlohmann::json ptsArr = nlohmann::json::array();
				for (size_t pi = 0; pi < lane.points.size(); pi++)
				{
					const AutomationPoint& pt = lane.points[pi];
					nlohmann::json p;
					p["timeSamples"] = pt.timeSamples;
					p["value"] = pt.value;
					ptsArr.push_back(p);
				}
				a["points"] = ptsArr;
				autoArr.push_back(a);
			}
			t["automation"] = autoArr;
			j["tracks"].push_back(t);
		}
		nlohmann::json markersArr = nlohmann::json::array();
		for (size_t mi = 0; mi < song.markers.size(); mi++)
		{
			const Marker& marker = song.markers[mi];
			nlohmann::json m;
			m["name"] = marker.name;
			m["timeSamples"] = marker.timeSamples;
			markersArr.push_back(m);
		}
		j["markers"] = markersArr;
		return j.dump(2);
	}

	static float getFloat(const nlohmann::json& j, const char* key, float defaultVal)
	{
		auto it = j.find(key);
		if (it != j.end() && it->is_number())
			return it->get<float>();
		return defaultVal;
	}

	static int64_t getInt64(const nlohmann::json& j, const char* key, int64_t defaultVal)
	{
		auto it = j.find(key);
		if (it != j.end() && it->is_number_integer())
			return it->get<int64_t>();
		return defaultVal;
	}

	static int getInt(const nlohmann::json& j, const char* key, int defaultVal)
	{
		auto it = j.find(key);
		if (it != j.end() && it->is_number_integer())
			return it->get<int>();
		return defaultVal;
	}

	Song JsonIO::FromJson(const std::string& jsonStr)
	{
		nlohmann::json j = nlohmann::json::parse(jsonStr);
		Song song;
		song.version = getInt(j, "version", 1);
		song.sampleRate = getInt(j, "sampleRate", 44100);

		auto tempoIt = j.find("tempo");
		if (tempoIt != j.end() && tempoIt->is_object())
		{
			song.tempo.bpm = getFloat(*tempoIt, "bpm", 128.0);
		}

		auto tracksIt = j.find("tracks");
		if (tracksIt != j.end() && tracksIt->is_array())
		{
			for (size_t ti = 0; ti < tracksIt->size(); ti++)
			{
				const nlohmann::json& t = (*tracksIt)[ti];
				Track track;
				track.name = "";
				auto nameIt = t.find("name");
				if (nameIt != t.end() && nameIt->is_string())
					track.name = nameIt->get<std::string>();

				auto devIt = t.find("device");
				if (devIt != t.end() && devIt->is_object())
				{
					auto typeIt = devIt->find("type");
					if (typeIt != devIt->end() && typeIt->is_string())
						track.device.type = typeIt->get<std::string>();

					auto paramsIt = devIt->find("parameters");
					if (paramsIt != devIt->end() && paramsIt->is_object())
					{
						for (auto pit = paramsIt->begin(); pit != paramsIt->end(); ++pit)
						{
							if (pit->is_number())
							{
								std::string key = pit.key();
								track.device.parameters[key] = pit->get<float>();
							}
						}
					}
				}

				auto notesIt = t.find("notes");
				if (notesIt != t.end() && notesIt->is_array())
				{
					for (size_t ni = 0; ni < notesIt->size(); ni++)
					{
						const nlohmann::json& n = (*notesIt)[ni];
						Note note;
						note.timeSamples = getInt64(n, "timeSamples", 0);
						note.durationSamples = getInt64(n, "durationSamples", 0);
						note.note = getInt(n, "note", 60);
						note.velocity = getFloat(n, "velocity", 1.0f);
						track.notes.push_back(note);
					}
				}

				auto autoIt = t.find("automation");
				if (autoIt != t.end() && autoIt->is_array())
				{
					for (size_t ai = 0; ai < autoIt->size(); ai++)
					{
						const nlohmann::json& a = (*autoIt)[ai];
						AutomationLane lane;
						auto targetIt = a.find("target");
						if (targetIt != a.end() && targetIt->is_string())
							lane.target = targetIt->get<std::string>();
						auto interpIt = a.find("interpolation");
						if (interpIt != a.end() && interpIt->is_string())
							lane.interpolation = interpIt->get<std::string>();

						auto ptsIt = a.find("points");
						if (ptsIt != a.end() && ptsIt->is_array())
						{
							for (size_t pi = 0; pi < ptsIt->size(); pi++)
							{
								const nlohmann::json& pt = (*ptsIt)[pi];
								AutomationPoint ap;
								ap.timeSamples = getInt64(pt, "timeSamples", 0);
								ap.value = getFloat(pt, "value", 0.0f);
								lane.points.push_back(ap);
							}
						}
						track.automation.push_back(lane);
					}
				}
				song.tracks.push_back(track);
			}
		}

		auto markersIt = j.find("markers");
		if (markersIt != j.end() && markersIt->is_array())
		{
			for (size_t mi = 0; mi < markersIt->size(); mi++)
			{
				const nlohmann::json& m = (*markersIt)[mi];
				Marker marker;
				auto nameIt = m.find("name");
				if (nameIt != m.end() && nameIt->is_string())
					marker.name = nameIt->get<std::string>();
				marker.timeSamples = getInt64(m, "timeSamples", 0);
				song.markers.push_back(marker);
			}
		}
		return song;
	}
}