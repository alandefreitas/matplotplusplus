#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;

    auto [x, y, z] = peaks();
    contourf(x, y, z);
    colorbar()
        .limits({-6, 8})
        .tick_values({-5, -2, 1, 4, 7})
        .ticklabels({"Cold", "Cool", "Neutral", "Warm", "Hot"});

    show();
    return 0;
}