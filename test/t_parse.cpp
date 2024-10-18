#include <iostream>
#include <fstream>

#include "jjson/json.h"

using namespace std;
char s[1000];

int main() {
    using namespace ::jjson::jjson_utils;
    "/Users/aythsr/Documents/CPP-code/JJSON/test/t.json"_f_json.export_to_file("./s.json");
    
    return 0;
}