set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE CORE ./ *.cpp)
add_executable(cpp ${CORE})
