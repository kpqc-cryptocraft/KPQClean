add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(test_rbc_vspace test_rbc_vspace.cpp)

target_link_libraries(test_rbc_vspace PUBLIC rbc)
target_link_libraries(test_rbc_vspace PUBLIC OpenSSL::Crypto)
target_link_libraries(test_rbc_vspace PRIVATE ${NTL_LIB})
target_link_libraries(test_rbc_vspace PRIVATE ${GMP_LIB})

target_include_directories(test_rbc_vspace PUBLIC ${RBC_LIB_INCLUDE})
target_include_directories(test_rbc_vspace PUBLIC ${RANDOM_SOURCE_DIR})
target_include_directories(test_rbc_vspace PUBLIC ${CMAKE_SOURCE_DIR}/../../../lib/minunit)

