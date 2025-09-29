add_library(stb STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/stb.cpp)
target_include_directories(stb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
