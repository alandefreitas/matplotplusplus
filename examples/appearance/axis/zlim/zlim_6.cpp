#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2,1);

    auto x = randn(50,0,1);
    auto y = randn(50,0,1);
    auto z = randn(50,0,1);
    scatter3(x,y,z);

    auto zl = zlim();
    std::cout << "zl[0]: " << zl[0] << std::endl;
    std::cout << "zl[1]: " << zl[1] << std::endl;

    wait();
    return 0;
}