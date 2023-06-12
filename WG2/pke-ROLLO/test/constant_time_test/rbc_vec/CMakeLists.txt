add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(test_cst_rbc_vec test_rbc_vec.c)

target_link_libraries(test_cst_rbc_vec PUBLIC rbc)
target_link_libraries(test_cst_rbc_vec PUBLIC OpenSSL::Crypto)
target_link_libraries(test_cst_rbc_vec PRIVATE ${NTL_LIB})
target_link_libraries(test_cst_rbc_vec PRIVATE ${GMP_LIB})

target_include_directories(test_cst_rbc_vec PUBLIC ${RBC_LIB_INCLUDE})
