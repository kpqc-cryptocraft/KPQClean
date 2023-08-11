add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(test_rbc_poly test_rbc_poly.cpp)

target_link_libraries(test_rbc_poly PUBLIC rbc)
target_link_libraries(test_rbc_poly PUBLIC OpenSSL::Crypto)
target_link_libraries(test_rbc_poly PRIVATE ${NTL_LIB})
target_link_libraries(test_rbc_poly PRIVATE ${GMP_LIB})

target_include_directories(test_rbc_poly PUBLIC ${RBC_LIB_INCLUDE})
target_include_directories(test_rbc_poly PUBLIC ${CMAKE_SOURCE_DIR}/../../../lib/minunit)

