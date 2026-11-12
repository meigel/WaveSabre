cmake_minimum_required(VERSION 3.11)
project("WaveSabreHeadless" LANGUAGES C CXX)

find_package(nlohmann_json 3.11 REQUIRED)

add_executable(wsir-render
	src/main.cpp
	src/OfflineRenderer.cpp)

target_link_libraries(wsir-render PRIVATE WaveSabreCore WaveSabreIR nlohmann_json::nlohmann_json)
target_include_directories(wsir-render PRIVATE include)
target_include_directories(wsir-render PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)