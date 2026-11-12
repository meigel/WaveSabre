add_library(WaveSabrePlayerLib
	include/WaveSabrePlayerLib/IPlayer.h
	include/WaveSabrePlayerLib/SongRenderer.h
	include/WaveSabrePlayerLib/WavWriter.h
	src/IPlayer.cpp
	src/WavWriter.cpp)

if(WIN32)
	target_sources(WaveSabrePlayerLib PRIVATE
		src/SongRenderer.cpp
		src/SongRenderer.Track.cpp
		src/CriticalSection.cpp
		src/DirectSoundRenderThread.cpp
		src/RealtimePlayer.cpp
		src/PreRenderPlayer.cpp)
	target_link_libraries(WaveSabrePlayerLib PRIVATE winmm.lib dsound.lib)
else()
	target_sources(WaveSabrePlayerLib PRIVATE
		src/SongRenderer.SingleThread.cpp)
endif()

target_link_libraries(WaveSabrePlayerLib PRIVATE WaveSabreCore)

target_include_directories(WaveSabrePlayerLib PUBLIC include)
target_include_directories(WaveSabrePlayerLib SYSTEM PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../WaveSabreCore/include)

if(MSVC)
	target_compile_definitions(WaveSabrePlayerLib PRIVATE _CRT_SECURE_NO_WARNINGS)
	target_compile_options(WaveSabrePlayerLib PUBLIC
		$<$<CONFIG:MinSizeRel>:/EHs-c->)
	set_property(TARGET WaveSabrePlayerLib APPEND_STRING PROPERTY STATIC_LIBRARY_FLAGS_MINSIZEREL
		" /LTCG")
endif()
