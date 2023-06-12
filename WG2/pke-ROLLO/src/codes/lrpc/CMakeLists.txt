
add_library(lrpc_ OBJECT rbc_lrpc.c)
target_include_directories(lrpc_ PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(lrpc_ PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(lrpc_ PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(lrpc_ PUBLIC ${CORE_DIR})
target_include_directories(lrpc_ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
