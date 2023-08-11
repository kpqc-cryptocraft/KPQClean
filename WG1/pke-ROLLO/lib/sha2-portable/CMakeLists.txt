add_library(sha2 OBJECT sha2.c)
target_include_directories(sha2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
