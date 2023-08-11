add_library(random_source OBJECT random_source.c)

target_include_directories(random_source PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(random_source PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(random_source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
