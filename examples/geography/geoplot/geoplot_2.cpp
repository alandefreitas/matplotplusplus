#include <matplot/matplot.h>

#include <cmath>

int main() {
    using namespace matplot;

    double lat_seattle = 47.62;
    double lon_seattle = -122.33;
    double lat_anchorage = 61.20;
    double lon_anchorage = -149.9;
    geoplot(std::vector{lat_seattle, lat_anchorage}, std::vector{lon_seattle, lon_anchorage}, "g-*");
    geolimits({45, 62}, {-155, -120});

    text(lon_anchorage, lat_anchorage, "Anchorage");
    text(lon_seattle, lat_seattle, "Seattle");

    wait();
    return 0;
}