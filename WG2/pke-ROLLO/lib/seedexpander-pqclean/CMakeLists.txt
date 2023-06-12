add_library(seedexpander OBJECT seedexpander.c)
target_include_directories(seedexpander PUBLIC ${AES_DIR})
target_include_directories(seedexpander PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
