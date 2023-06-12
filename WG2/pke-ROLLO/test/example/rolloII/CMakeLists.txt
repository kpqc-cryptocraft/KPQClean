add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(rolloII main.c)
target_link_libraries(rolloII PUBLIC rbc)

target_include_directories(rolloII PUBLIC ${RBC_LIB_INCLUDE})

