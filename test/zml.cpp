#include <iostream>

using namespace std;


namespace ss {

int operator ""_int (const char * s, size_t n) {
    return int(n);
}

}

int main() {
    using ss::operator""_int;
    cout << "123"_int << endl;
    return 0;
}