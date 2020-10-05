#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    gcf()->quiet_mode(true);
    auto z = linspace(0., 4. * pi, 250);
    auto x = transform(z, [](double z) { return 2 * cos(z) + rand(0, 1); });
    auto y = transform(z, [](double z) { return 2 * sin(z) + rand(0, 1); });

    std::vector<double> sizes(x.size(), 5);
    std::vector<double> colors(x.size() / 2, 1);
    std::fill_n(std::back_inserter(colors), x.size() / 2, 2);

    scatter3(x, y, z, sizes, colors, "filled");
    colormap(palette::paired());

    gca()->visible(false);
    view(-30, 10);

    show();
    return 0;
}