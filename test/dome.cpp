#include <iostream>

using namespace std;

int a;

struct ap {
    int* a;
    ap() : a(nullptr) {}
    ap(int n) {
        cout << n << " creat" << endl;
        a = new int[n];
    }
    ~ap() {
        if (a == nullptr) {
            cout << "null" << endl;
        }
        delete[] a;
    }
};

struct node {
    ap p;
    void get(node&& x) {
        cout << "move" << endl;
        p.a = x.p.a;
        x.p.a = nullptr;
    }  
    void get(const node& x) {
        cout << "copy" << endl;
    }
    node(int n) : p(n) {};
    node() {}
};


int main() {
    // pp(a);
    node a;
    a.get(move(node(5)));
    cout << "111" << endl;
    // cout << a.p->a << ' ' << a.p->b << endl;
    return 0;
}