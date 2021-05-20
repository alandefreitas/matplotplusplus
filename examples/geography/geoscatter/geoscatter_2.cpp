#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> lon = iota(-170, 10, 170);
    std::vector<double> lat =
        transform(lon, [](double lon) { return 50. * cosd(3 * lon); });
    std::vector<double> A = transform(
        lon, [](double lon) { return (101. + 100 * (sind(2 * lon))) / 7; });
    std::vector<double> C =
        transform(lon, [](double lon) { return cosd(4 * lon); });

    geoscatter(lat, lon, A, C)
        ->marker_style(line_spec::marker_style::upward_pointing_triangle);

    color_array terrain = {0.f, 0.71f, 0.65f, 0.59f};
    geoplot()->color(terrain);

    color_array blue_water = {0.f, 0.4f, 0.61f, 0.76f};
    gca()->color(blue_water);

    show();
    return 0;
}