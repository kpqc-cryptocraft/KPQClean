add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(biix main.c)
target_link_libraries(biix PUBLIC rbc)

target_include_directories(biix PUBLIC ${RBC_LIB_INCLUDE})
