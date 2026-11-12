cmake_minimum_required(VERSION 3.11)
project("WaveSabre" LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
	string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

option(WAVESABRE_BUILD_CORE "Build WaveSabre DSP core" ON)
option(WAVESABRE_BUILD_PLAYERLIB "Build WaveSabre player library" ON)
option(WAVESABRE_BUILD_HEADLESS "Build headless renderer and tools" ON)
option(WAVESABRE_BUILD_IR "Build WaveSabre IR library" ON)
option(WAVESABRE_BUILD_DAWPROJECT "Build DAWproject importer" ON)
option(WAVESABRE_BUILD_CLAP "Build CLAP plugin" ON)

option(WAVESABRE_BUILD_WIN32_AUDIO "Build DirectSound/WinMM realtime audio backend" OFF)
option(WAVESABRE_BUILD_LEGACY_VST2 "Build legacy VST2 plugins" OFF)
option(WAVESABRE_BUILD_DOTNET_TOOLS "Build legacy C# tools" OFF)
option(WAVESABRE_BUILD_PROJECT_MANAGER "Build WinForms ProjectManager" OFF)
option(WAVESABRE_ENABLE_GSM_ACM "Enable Windows ACM GSM sample decoding" OFF)

set(VSTSDK3_DIR "${PROJECT_SOURCE_DIR}/Vst3.x/" CACHE PATH "VSTSDK location")

if(WAVESABRE_BUILD_DOTNET_TOOLS AND NOT WIN32)
	message(FATAL_ERROR "WAVESABRE_BUILD_DOTNET_TOOLS requires Windows")
endif()

if(WAVESABRE_BUILD_PROJECT_MANAGER AND NOT WIN32)
	message(FATAL_ERROR "WAVESABRE_BUILD_PROJECT_MANAGER requires Windows")
endif()

if(WAVESABRE_BUILD_WIN32_AUDIO AND NOT WIN32)
	message(FATAL_ERROR "WAVESABRE_BUILD_WIN32_AUDIO requires Windows")
endif()

if(WAVESABRE_BUILD_LEGACY_VST2 AND NOT WIN32)
	message(FATAL_ERROR "WAVESABRE_BUILD_LEGACY_VST2 requires Windows")
endif()

if(WAVESABRE_BUILD_CORE)
	add_subdirectory(WaveSabreCore)
endif()

if(WAVESABRE_BUILD_PLAYERLIB)
	add_subdirectory(WaveSabrePlayerLib)
endif()

if(WAVESABRE_BUILD_IR AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WaveSabreIR")
	add_subdirectory(WaveSabreIR)
endif()

if(WAVESABRE_BUILD_DAWPROJECT AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WaveSabreDawProject")
	add_subdirectory(WaveSabreDawProject)
endif()

if(WAVESABRE_BUILD_CLAP AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WaveSabreClap")
	add_subdirectory(WaveSabreClap)
endif()

if(WAVESABRE_BUILD_HEADLESS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WaveSabreHeadless")
	add_subdirectory(WaveSabreHeadless)
endif()

if(WAVESABRE_BUILD_CORE)
	if(WIN32)
		add_subdirectory(Tests/PlayerTest)
	endif()
endif()

if(WAVESABRE_BUILD_LEGACY_VST2)
	set(BUILD_VST_PLUGINS ON)
endif()

# VST3 plugins
if(BUILD_VST_PLUGINS AND VSTSDK3_DIR AND WIN32)
	add_subdirectory(WaveSabreVstLib)
	add_subdirectory(Vsts)
endif()

# Legacy C# tools
if(WAVESABRE_BUILD_DOTNET_TOOLS AND WIN32)
	add_subdirectory(WaveSabreConvert)
	if(WAVESABRE_BUILD_CONVERT_TESTS)
		add_subdirectory(Tests/WaveSabreConvertTests)
	endif()
	add_subdirectory(ConvertTheFuck)
endif()

# WinForms project manager
if(WAVESABRE_BUILD_PROJECT_MANAGER AND WIN32)
	add_subdirectory(ProjectManager)
endif()
