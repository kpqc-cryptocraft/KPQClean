add_library(sha2-openssl OBJECT sha2.c)

find_package(OpenSSL REQUIRED)
target_link_libraries(sha2-openssl OpenSSL::Crypto)

target_include_directories(sha2-openssl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
