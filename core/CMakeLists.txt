set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CORE ./ *.cpp)
add_executable(cpp ${CORE})
target_link_libraries(
    cpp PRIVATE
    nlohmann_json::nlohmann_json
    glm
)
