
add_library(gabidulin_ OBJECT rbc_gabidulin.c rbc_qpoly.c)
target_include_directories(gabidulin_ PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(gabidulin_ PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(gabidulin_ PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(gabidulin_ PUBLIC ${CORE_DIR})
target_include_directories(gabidulin_ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

