#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    mesh(x, y, z);
    colormap(palette::autumn(5));
    auto cmap = colormap();
    for (const auto &color : cmap) {
        std::cout << "r: " << color[0] << ", g: " << color[1]
                  << ", b: " << color[2] << std::endl;
    }

    show();
    return 0;
}