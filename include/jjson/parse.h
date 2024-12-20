#pragma once

// #include <iostream>
#include <fstream>

#include "jjson.h"

// std::string get_string(const char* &p, const char *end);

namespace jjson {

namespace jjson_utils {

    json_t operator ""_json (const char *, size_t);
    json_t operator ""_f_json (const char *, size_t);

}

/**
 * 从字符串中加载json文件
 */
json_t parse(const char *, const char *);

/**
 * 从字符串中加载json文件
 */
json_t parse(const string& x);

using ::std::ifstream;

/**
 * parse from a fstream
 */
json_t parse(ifstream& x);

/**
 * parse from a filepath
 */
json_t parse(const char * filepath);
}