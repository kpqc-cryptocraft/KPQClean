add_library(randombytes-nist OBJECT randombytes.c)

find_package(OpenSSL REQUIRED)
target_link_libraries(randombytes-nist OpenSSL::Crypto)

target_include_directories(randombytes-nist PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
