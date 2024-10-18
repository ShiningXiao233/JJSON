
add_library(jjson jjson.cc parse.cc)

target_include_directories(jjson PUBLIC ${include_dir})