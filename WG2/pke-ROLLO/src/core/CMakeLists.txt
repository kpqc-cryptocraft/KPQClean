
add_library(core OBJECT rbc_elt.c rbc_vec.c rbc_poly.c rbc_vspace.c rbc_qre.c)
target_include_directories(core PUBLIC ${RANDOMBYTES_DIR})
target_include_directories(core PUBLIC ${SEEDEXPANDER_DIR})
target_include_directories(core PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
