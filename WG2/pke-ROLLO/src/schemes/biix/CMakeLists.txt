
add_library(biix OBJECT biix_parsing.c biix_kem.c)

target_include_directories(biix PUBLIC ${SHA2_DIR})
target_include_directories(biix PUBLIC ${FIPS202_DIR})
target_include_directories(biix PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(biix PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(biix PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(biix PUBLIC ${CORE_DIR})
target_include_directories(biix PUBLIC ${LRPC_DIR})
target_include_directories(biix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
