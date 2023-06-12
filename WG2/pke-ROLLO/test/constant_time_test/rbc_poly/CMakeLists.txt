add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(test_cst_rbc_poly test_rbc_poly.c)

target_link_libraries(test_cst_rbc_poly PUBLIC rbc)
target_link_libraries(test_cst_rbc_poly PUBLIC OpenSSL::Crypto)
target_link_libraries(test_cst_rbc_poly PRIVATE ${NTL_LIB})
target_link_libraries(test_cst_rbc_poly PRIVATE ${GMP_LIB})

target_include_directories(test_cst_rbc_poly PUBLIC ${RBC_LIB_INCLUDE})

