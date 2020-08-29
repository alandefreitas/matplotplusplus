#include <matplot/matplot.h>

#include <cmath>

int main() {
  using namespace matplot;

  auto funx = [](double s, double t) { return sin(s); };
  auto funy = [](double s, double t) { return cos(s); };
  auto funz = [](double s, double t) { return t / 10 * sin(1. / s); };

  tiledlayout(2, 1);
  nexttile();
  fsurf(funx, funy, funz, std::array<double, 2>{-5, +5}, "", 20);
  view(-172, 25);
  title("Decreased Mesh Density");

  nexttile();
  fsurf(funx, funy, funz, std::array<double, 2>{-5, +5}, "", 80);
  view(-172, 25);
  title("Increased Mesh Density");

  wait();
  return 0;
}