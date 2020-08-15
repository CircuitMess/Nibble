cmake_minimum_required(VERSION 3.15)
project(Nibble)

set(CMAKE_CXX_STANDARD 14)

include_directories(~/.arduino15/packages/esp8266/hardware/esp8266/2.7.1/cores/esp8266)

include_directories(~/Arduino/libraries/CircuitOS/src)
include_directories(~/Arduino/libraries/TFT_eSPI)
include_directories(~/Arduino/libraries/TFT_eSPI/Extensions)

AUX_SOURCE_DIRECTORY(src SOURCES)

add_definitions(-DDEBUG)
add_definitions(-DCIRCUITOS_LITTLEFS)
add_definitions(-DCIRCUITOS_TONE)
add_definitions(-DCIRCUITOS_LOWRAM)

SET_SOURCE_FILES_PROPERTIES(Nibble.ino PROPERTIES LANGUAGE CXX)
add_executable(Nibble ${SOURCES} Nibble.ino)