cmake_minimum_required(VERSION 3.12)
project(advpt_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(advpt_project src/main.cpp src/headers/BuildFactoryEvent.hpp src/headers/DestroyFactoryEvent.hpp src/headers/Factory.hpp src/headers/Recipe.hpp
        src/headers/ResearchEvent.hpp src/headers/StartFactoryEvent.hpp src/headers/StopFactoryEvent.hpp src/headers/Technology.hpp src/headers/VictoryEvent.hpp
        src/headers/Event.hpp src/source/BuildFactoryEvent.cpp src/source/DestroyFactoryEvent.cpp src/source/Factory.cpp src/source/Recipe.cpp src/source/ResearchEvent.cpp
        src/source/StartFactoryEvent.cpp src/source/StopFactoryEvent.cpp src/source/Technology.cpp src/source/VictoryEvent.cpp src/source/Event.cpp src/source/Utils.cpp src/headers/Utils.hpp
        src/headers/SolLoader.hpp src/source/SolLoader.cpp src/source/Solution.cpp src/headers/Solution.hpp src/headers/GameEntity.hpp src/source/GameEntity.cpp)

#adding lib
add_subdirectory(lib)

target_link_libraries(advpt_project PRIVATE nlohmann)
target_link_libraries(advpt_project PRIVATE tree)
set_target_properties(advpt_project PROPERTIES OUTPUT_NAME "generator")
