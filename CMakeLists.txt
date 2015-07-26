cmake_minimum_required(VERSION 3.2)
project(rocketcats)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "-std=c++14")

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_TTF REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)
add_executable(SDL2_App ${SOURCE_FILES} src/World/Cat.cpp src/World/Map.cpp src/Game.cpp src/Event.cpp src/Camera.cpp src/Timer.cpp src/FatalGameException.cpp src/World/MapGenerator.cpp src/World/Team.cpp src/World/NinjaRope.cpp src/Perlin.cpp src/World/Explosion.cpp src/World/Tile.cpp src/World/Grenade.cpp src/World/Entity.cpp src/World/Mine.cpp)
target_link_libraries(SDL2_App ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})