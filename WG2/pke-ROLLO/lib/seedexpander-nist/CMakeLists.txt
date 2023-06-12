add_library(seedexpander-nist OBJECT seedexpander.c)

find_package(OpenSSL REQUIRED)
target_link_libraries(seedexpander-nist OpenSSL::Crypto)

target_include_directories(seedexpander-nist PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
