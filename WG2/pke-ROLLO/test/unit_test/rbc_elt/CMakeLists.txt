add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(test_rbc_elt test_rbc_elt.cpp)

target_link_libraries(test_rbc_elt PUBLIC rbc)
target_link_libraries(test_rbc_elt PUBLIC OpenSSL::Crypto)
target_link_libraries(test_rbc_elt PRIVATE ${NTL_LIB})
target_link_libraries(test_rbc_elt PRIVATE ${GMP_LIB})

target_include_directories(test_rbc_elt PUBLIC ${RBC_LIB_INCLUDE})
target_include_directories(test_rbc_elt PUBLIC ${CMAKE_SOURCE_DIR}/../../../lib/minunit)
