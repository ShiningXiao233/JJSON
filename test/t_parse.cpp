#include <iostream>
#include <fstream>

#include "jjson/json.h"

using namespace std;
char s[1000];

int main() {
    ifstream f("/Users/aythsr/Documents/CPP-code/JJSON/test/t.json");
    jjson::json_t js = jjson::parse(f);
    ofstream ff("./t.json");
    ff << js.to_string() << endl;
    return 0;
}