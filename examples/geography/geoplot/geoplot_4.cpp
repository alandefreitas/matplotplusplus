#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    double lat_seattle = 47.62;
    double lon_seattle = -122.33;
    double lat_anchorage = 61.20;
    double lon_anchorage = -149.9;
    double lat_pt_barrow = 71.38;
    double lon_pt_barrow = -156.47;

    geoplot(std::vector{lat_seattle, lat_anchorage},
            std::vector{lon_seattle, lon_anchorage}, "y-");
    hold(on);
    geoplot(std::vector{lat_seattle, lat_pt_barrow},
            std::vector{lon_seattle, lon_pt_barrow}, "b:");

    geolimits(44, 75, -170, -100);

    text(lon_anchorage, lat_anchorage, "Anchorage");
    text(lon_seattle, lat_seattle, "Seattle");
    text(lon_pt_barrow, lat_pt_barrow, "Point Barrow");

    show();
    return 0;
}