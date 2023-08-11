add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(rolloI main.c)
target_link_libraries(rolloI PUBLIC rbc)

target_include_directories(rolloI PUBLIC ${RBC_LIB_INCLUDE})
