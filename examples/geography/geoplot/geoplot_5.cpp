#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  double lat_seattle = 47.62;
  double lon_seattle = -122.33;
  double lat_anchorage = 61.20;
  double lon_anchorage = -149.9;

  auto g = geoplot(std::vector{lat_seattle, lat_anchorage},
                   std::vector{lon_seattle, lon_anchorage});
  g->line_width(2.);
  hold(on);

  geolimits({44, 75}, {-170, -100});

  text(lon_anchorage, lat_anchorage, "Anchorage");
  text(lon_seattle, lat_seattle, "Seattle");

  color_array terrain = {0., 0.71, 0.65, 0.59};
  geoplot()->color(terrain);

  color_array blue_water = {0., 0.4, 0.61, 0.76};
  gca()->color(blue_water);

  wait();
  return 0;
}