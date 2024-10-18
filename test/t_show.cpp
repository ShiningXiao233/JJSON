#include <iostream>
#include <fstream>

#include "jjson/jjson.h"

int main() {
    using jjson::json_t;
    using jjson::json_val_type;

    std::cout << "++++" << std::endl;

    json_t a(json_val_type::Dict);

    std::cout << "---" << std::endl;
    a.set_key_val("key", new json_t("val"));
    std::cout << "---" << std::endl;

    a.set_key_val("array", json_t(json_val_type::Array));
    a["array"].array_appond_val(json_t(json_val_type::Array));
    a["array"][0].array_appond_val(json_t("1"));
    a["array"][0].array_appond_val(json_t("2"));
    a["array"][0].array_appond_val(json_t("3"));
    a["array"][0].array_appond_val(json_t("4"));
    a["array"].array_appond_val(json_t("11"));
    a["array"].array_appond_val(json_t("22"));
    a["array"].array_appond_val(json_t("33"));
    a["array"].array_appond_val(json_t(json_val_type::Array));
    a["array"][4].array_appond_val(json_t("s1"));
    a["array"][4].array_appond_val(json_t("s2"));
    a["array"][4].array_appond_val(json_t("s3"));
    a.set_key_val("dict", json_t(json_val_type::Dict));
    a["dict"].set_key_val("k1", json_t("v1"));
    a["dict"].set_key_val("k2", json_t("v1"));
    std::cout << a[1]["sss"][3][4]["sdsa"].get_string() << std::endl;
    std::cout << "======" << std::endl;
    std::ofstream f("./t.json");
    f << a.to_string() << std::endl;
    json_t b(a);
    std::ofstream bf("./tt.json");
    bf << b.to_string() << std::endl;
    // f << a["array"].to_string() << std::endl;
    // f << a["array"][1].get_string() << std::endl;
    
    return 0;
}