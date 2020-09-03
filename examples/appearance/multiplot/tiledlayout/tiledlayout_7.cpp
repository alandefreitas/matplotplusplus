#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(1, 2);

    nexttile();
    geoplot(vector_1d{47.62, 61.20}, vector_1d{-122.33, -149.90}, "g-*");
    geolimits({46, 62}, {-160, -110});

    nexttile();
    auto theta = iota(pi / 4, pi / 4, 2 * pi);
    std::vector<double> rho = {19, 6, 12, 18, 16, 11, 15, 15};
    polarscatter(theta, rho);

    show();
    return 0;
}