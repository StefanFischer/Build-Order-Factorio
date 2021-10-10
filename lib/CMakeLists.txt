add_library(nlohmann INTERFACE)
target_sources(nlohmann INTERFACE ${CMAKE_CURRENT_LIST_DIR}/nlohmann/nlohmann_json.hpp)

target_include_directories(nlohmann INTERFACE
        nlohmann)

add_library(tree INTERFACE)
target_sources(nlohmann INTERFACE ${CMAKE_CURRENT_LIST_DIR}/tree/tree.hh)
target_sources(nlohmann INTERFACE ${CMAKE_CURRENT_LIST_DIR}/tree/tree_util.hh)
target_include_directories(tree INTERFACE tree)