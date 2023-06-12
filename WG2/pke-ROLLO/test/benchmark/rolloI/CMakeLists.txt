add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(benchmark-rolloI main.c)
target_link_libraries(benchmark-rolloI PUBLIC rbc)

find_package(OpenSSL REQUIRED)
target_link_libraries(benchmark-rolloI PUBLIC OpenSSL::Crypto)

target_include_directories(benchmark-rolloI PUBLIC ${RBC_LIB_INCLUDE})

