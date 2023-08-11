
add_library(rolloII OBJECT rolloII_parsing.c rolloII_kem.c)

target_include_directories(rolloII PUBLIC ${SHA2_DIR})
target_include_directories(rolloII PUBLIC ${FIPS202_DIR})
target_include_directories(rolloII PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(rolloII PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(rolloII PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(rolloII PUBLIC ${CORE_DIR})
target_include_directories(rolloII PUBLIC ${LRPC_DIR})
target_include_directories(rolloII PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
