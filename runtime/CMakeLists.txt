add_library(tiny_runtime STATIC runtime.cpp)
target_include_directories(tiny_runtime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
