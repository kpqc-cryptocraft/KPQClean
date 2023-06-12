add_library(fips202 OBJECT fips202.c)
target_include_directories(fips202 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
