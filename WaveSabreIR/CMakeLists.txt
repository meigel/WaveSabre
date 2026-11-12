find_package(nlohmann_json 3.11 REQUIRED)

add_library(WaveSabreIR
	include/WaveSabreIR/Song.h
	include/WaveSabreIR/Device.h
	include/WaveSabreIR/Track.h
	include/WaveSabreIR/Note.h
	include/WaveSabreIR/AutomationLane.h
	include/WaveSabreIR/JsonIO.h
	src/Song.cpp
	src/JsonIO.cpp)

target_link_libraries(WaveSabreIR PRIVATE WaveSabreCore nlohmann_json::nlohmann_json)
target_include_directories(WaveSabreIR PUBLIC include)