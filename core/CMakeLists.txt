set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CORE ./ *.cpp)
add_executable(cpp ${CORE})