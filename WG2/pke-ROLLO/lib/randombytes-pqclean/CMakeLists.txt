add_library(randombytes OBJECT randombytes.c)
target_include_directories(randombytes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
