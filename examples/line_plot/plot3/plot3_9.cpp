#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::default_random_engine g(0);
    std::uniform_real_distribution<double> d;

    std::vector<double> x(10);
    std::generate(x.begin(), x.end(), [&]() { return d(g); });

    std::vector<double> y(10);
    std::generate(y.begin(), y.end(), [&]() { return d(g); });

    std::uniform_real_distribution<double> d2(0, 90);
    std::vector<double> z(10);
    std::generate(z.begin(), z.end(), [&]() { return d2(g); });

    auto p = plot3(x, y, z, "o");
    p->parent()->x_axis().tick_label_format("s");
    xlabel("X");
    ylabel("Y");
    zlabel("Duration");
    grid(on);

    show();
    return 0;
}