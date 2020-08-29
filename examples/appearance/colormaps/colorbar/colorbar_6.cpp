#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> map = {{0, 0, 0.3}, {0, 0, 0.4}, {0, 0, 0.5}, {0, 0, 0.6}, {0, 0, 0.8}, {0, 0, 1}};
    auto [x,y,z] = peaks();
    surf(x,y,z);
    colorbar().label("Elevation (ft in 1000s)");

    wait();
    return 0;
}