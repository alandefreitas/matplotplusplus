#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto [lon, lat, names] = world_cities(6, 8);
    auto [lon_star, lat_star] = greedy_tsp(lon, lat);
    geoplot(lat_star, lon_star)
        ->marker("o")
        .marker_colors(iota(1, names.size()));
    text(lon, lat, names);

    wait();
    return 0;
}