#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);

    auto x = randn(50, 0, 1);
    auto y = randn(50, 0, 1);
    auto z = randn(50, 0, 1);
    scatter3(x, y, z);

    auto zl = zlim();
    std::cout << "zl[0]: " << zl[0] << std::endl;
    std::cout << "zl[1]: " << zl[1] << std::endl;

    show();
    return 0;
}