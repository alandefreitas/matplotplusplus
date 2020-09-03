#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    auto z = linspace(0, 4 * pi, 250);
    auto x = transform(z, [](double z) { return 2 * cos(z) + rand(0, 1); });
    auto y = transform(z, [](double z) { return 2 * sin(z) + rand(0, 1); });

    scatter3(x, y, z, "*");
    view(-30, 10);

    show();
    return 0;
}