
add_library(rolloI OBJECT rolloI_parsing.c rolloI_kem.c)

target_include_directories(rolloI PUBLIC ${SHA2_DIR})
target_include_directories(rolloI PUBLIC ${FIPS202_DIR})
target_include_directories(rolloI PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(rolloI PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(rolloI PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(rolloI PUBLIC ${CORE_DIR})
target_include_directories(rolloI PUBLIC ${LRPC_DIR})
target_include_directories(rolloI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
