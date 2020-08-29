#include <matplot/matplot.h>

int main() {
  using namespace matplot;

  std::vector<std::vector<double>> Y = {
      {1, 3, 1, 2}, {5, 2, 5, 6}, {3, 7, 3, 1}};

  auto f = gcf();
  f->width(f->width() * 2);

  subplot(1, 2, 0);
  auto h1 = area(Y, -4.);
  h1[0]->line_style(":");
  h1[1]->line_style(":");
  h1[2]->line_style(":");
  h1[0]->face_color({0, 0, 0.25, 0.25});
  h1[1]->face_color({0, 0, 0.5, 0.5});
  h1[2]->face_color({0, 0, 0.75, 0.75});
  title("Stacked");

  subplot(1, 2, 1);
  auto h2 = area(Y, -4, false);
  h2[0]->line_style(":");
  h2[1]->line_style(":");
  h2[2]->line_style(":");
  h2[0]->face_color({0.2, 0, 0.25, 0.25});
  h2[1]->face_color({0.2, 0, 0.5, 0.5});
  h2[2]->face_color({0.2, 0, 0.75, 0.75});
  title("Not stacked");

  wait();
  return 0;
}