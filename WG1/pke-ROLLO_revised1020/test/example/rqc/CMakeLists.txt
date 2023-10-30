add_library(rbc STATIC IMPORTED)
set_target_properties(rbc PROPERTIES IMPORTED_LOCATION ${RBC_LIB_SOURCE})

add_executable(rqc main.c)
target_link_libraries(rqc PUBLIC rbc)

target_include_directories(rqc PUBLIC ${RBC_LIB_INCLUDE})

