
add_library(biiy OBJECT biiy_parsing.c biiy_kem.c)

target_include_directories(biiy PUBLIC ${SHA2_DIR})
target_include_directories(biiy PUBLIC ${FIPS202_DIR})
target_include_directories(biiy PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(biiy PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(biiy PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(biiy PUBLIC ${CORE_DIR})
target_include_directories(biiy PUBLIC ${LRPC_DIR})
target_include_directories(biiy PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
