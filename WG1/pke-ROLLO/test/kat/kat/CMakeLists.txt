add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(kat main.c)
target_link_libraries(kat PUBLIC rbc)

find_package(OpenSSL REQUIRED)
target_link_libraries(kat PUBLIC OpenSSL::Crypto)

target_include_directories(kat PUBLIC ${RBC_LIB_INCLUDE})

