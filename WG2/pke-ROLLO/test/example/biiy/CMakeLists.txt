add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(biiy main.c)
target_link_libraries(biiy PUBLIC rbc)

target_include_directories(biiy PUBLIC ${RBC_LIB_INCLUDE})
