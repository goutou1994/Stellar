//#include "Model.h"
#include <vector>
#include <iostream>


class test_class {
public:
    test_class(std::vector<int> &&v) : cv(v) {};
    int& operator[](int i) {
        return cv[i];
    }
private:
    std::vector<int> cv;
};


void test(std::vector<int> &&v) {
    std::cout << v[1] << std::endl;
}

int main() {

    std::vector<int> v = {1, 2, 3};
    test_class tc(std::move(v));
    std::cout << tc[2] << std::endl;
//    Model test("./Torus_Knot.obj");
    return 0;
}