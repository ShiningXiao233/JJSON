
# add_executable(t_show t_show.cpp)
add_executable(t_parse t_parse.cpp)

# target_include_directories(jjson PUBLIC ${include_dir})

target_link_libraries(t_parse PUBLIC jjson )
# target_link_libraries(t_show PUBLIC jjson)