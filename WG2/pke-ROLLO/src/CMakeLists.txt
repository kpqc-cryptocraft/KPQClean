
##### Global settings

cmake_minimum_required(VERSION 3.5)
project(rbc-lib)

set(rbc-lib_Version_MAJOR 0)
set(rbc-lib_Version_MINOR 1)



##### Build-dependant settings (from config.yml)

set(CORE_LAYER_IMPLEM "CONFIG_CORE_LAYER_IMPLEM")

set(RBC_LIB_INSTALL_PATH CONFIG_LIB_DIR)
set(RBC_LIB_INSTALL_PATH_INC CONFIG_LIB_DIR/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY CONFIG_LIB_DIR)



##### Compilation flags

if(CORE_LAYER_IMPLEM STREQUAL "avx")
  #set(CMAKE_C_FLAGS "-O3 -flto -march=skylake -mavx2 -mpclmul -msse4.2 -maes -std=c99 -pedantic -Wall -Wextra -Werror")
  set(CMAKE_C_FLAGS "-O3 -flto -mavx2 -mpclmul -msse4.2 -maes -std=c99 -pedantic -Wall -Wextra")
  #set(CMAKE_C_FLAGS "-O3 -mavx2 -mpclmul -msse4.2 -maes -std=c99 -pedantic -Wall -Wextra -Werror")
endif(CORE_LAYER_IMPLEM STREQUAL "avx")

if(CORE_LAYER_IMPLEM STREQUAL "c64")
  #set(CMAKE_C_FLAGS "-O3 -flto -march=skylake -std=c99 -pedantic -Wall -Wextra -Werror")
  set(CMAKE_C_FLAGS "-O3 -flto -std=c99 -pedantic -Wall -Wextra -Werror")
  #set(CMAKE_C_FLAGS "-O3 -std=c99 -pedantic -Wall -Wextra -Werror")
endif(CORE_LAYER_IMPLEM STREQUAL "c64")

if(CORE_LAYER_IMPLEM STREQUAL "c32")
  set(CMAKE_C_FLAGS "-O3 -flto -std=c99 -pedantic -Wall -Wextra -Werror")
endif(CORE_LAYER_IMPLEM STREQUAL "c32")


##### Compilation

set(SHA2_DIR                 ${CMAKE_CURRENT_SOURCE_DIR}/lib/sha2-portable)
set(FIPS202_DIR              ${CMAKE_CURRENT_SOURCE_DIR}/lib/fips202-portable)

if(CORE_LAYER_IMPLEM STREQUAL "avx")
  set(AES_DIR                ${CMAKE_CURRENT_SOURCE_DIR}/lib/aes-ni)
endif(CORE_LAYER_IMPLEM STREQUAL "avx")

if(CORE_LAYER_IMPLEM STREQUAL "c64")
  set(AES_DIR                ${CMAKE_CURRENT_SOURCE_DIR}/lib/aes-portable)
endif(CORE_LAYER_IMPLEM STREQUAL "c64")

if(CORE_LAYER_IMPLEM STREQUAL "c32")
  set(AES_DIR                ${CMAKE_CURRENT_SOURCE_DIR}/lib/aes-portable)
endif(CORE_LAYER_IMPLEM STREQUAL "c32")

set(RANDOM_SOURCE_DIR               ${CMAKE_CURRENT_SOURCE_DIR}/lib/random_source)
set(RANDOMBYTES_DIR          ${CMAKE_CURRENT_SOURCE_DIR}/lib/randombytes-pqclean)
set(SEEDEXPANDER_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/lib/seedexpander-pqclean)

set(SHA2_OPENSSL_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/lib/sha2-openssl)
set(RANDOMBYTES_NIST_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/lib/randombytes-nist)
set(SEEDEXPANDER_NIST_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/lib/seedexpander-nist)

{TEMPLATING_CMAKE_SET_DIR}

add_subdirectory(${AES_DIR})
add_subdirectory(${SHA2_DIR})
add_subdirectory(${FIPS202_DIR})
add_subdirectory(${RANDOMBYTES_DIR})
add_subdirectory(${RANDOM_SOURCE_DIR})
add_subdirectory(${SEEDEXPANDER_DIR})

add_subdirectory(${SHA2_OPENSSL_DIR})
add_subdirectory(${RANDOMBYTES_NIST_DIR})
add_subdirectory(${SEEDEXPANDER_NIST_DIR})

{TEMPLATING_CMAKE_ADD_SUBDIRECTORY}

add_library(rbc STATIC
  $<TARGET_OBJECTS:aes>
  $<TARGET_OBJECTS:sha2>
  $<TARGET_OBJECTS:fips202>
  $<TARGET_OBJECTS:random_source>
  $<TARGET_OBJECTS:randombytes>
  $<TARGET_OBJECTS:seedexpander>

{TEMPLATING_CMAKE_ADD_LIBRARY}

)

add_library(rbc-nist STATIC
  $<TARGET_OBJECTS:fips202>
  $<TARGET_OBJECTS:sha2-openssl>
  $<TARGET_OBJECTS:random_source>
  $<TARGET_OBJECTS:randombytes-nist>
  $<TARGET_OBJECTS:seedexpander-nist>

{TEMPLATING_CMAKE_ADD_LIBRARY}

)



##### Installation
set(RBC_LIB_SRC ${PROJECT_SOURCE_DIR}/src)
install(TARGETS rbc ARCHIVE DESTINATION ${RBC_LIB_INSTALL_PATH})
install(TARGETS rbc-nist ARCHIVE DESTINATION ${RBC_LIB_INSTALL_PATH})

install(FILES ${AES_DIR}/aes.h                                                    DESTINATION ${RBC_LIB_INSTALL_PATH_INC})
install(FILES ${SHA2_DIR}/sha2.h                                                  DESTINATION ${RBC_LIB_INSTALL_PATH_INC})
install(FILES ${FIPS202_DIR}/fips202.h                                            DESTINATION ${RBC_LIB_INSTALL_PATH_INC})
install(FILES ${RANDOM_SOURCE_DIR}/random_source.h                                              DESTINATION ${RBC_LIB_INSTALL_PATH_INC})
install(FILES ${RANDOMBYTES_NIST_DIR}/randombytes.h                               DESTINATION ${RBC_LIB_INSTALL_PATH_INC})
install(FILES ${SEEDEXPANDER_DIR}/seedexpander.h                                  DESTINATION ${RBC_LIB_INSTALL_PATH_INC})

{TEMPLATING_CMAKE_INSTALL_FILES}

install(FILES ${RBC_LIB_SRC}/include/rbc.h                                        DESTINATION ${RBC_LIB_INSTALL_PATH_INC})

