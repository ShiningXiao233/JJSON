#include <iostream>
#include <fstream>

#include "jjson/json.h"

using namespace std;
char s[1000];

int main() {
    ifstream f("/home/aurora/Documents/sm-web-server/test/jjson/test/t.json");
    char c;
    int len = 0;
    while (f >> c) {
        s[len ++] = c;
    }
    cout << s << endl;
    jjson::json_t js = jjson::parse(s, s + len);
    ofstream ff("./t.json");
    ff << js.to_string() << endl;
    ofstream _f("./_t.json");
    using jjson::jjson_utils::operator""_json;
    auto b = "{\"array\":[1,2,3,4],\"dict\":{}}"_json;
    _f << b.to_string();
    cout << b["array"][1].to_string() << endl;
    cout << js["dict"].to_string() << endl;

    return 0;
}