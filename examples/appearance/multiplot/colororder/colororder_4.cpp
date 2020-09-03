#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> newcolors = {{0.4, 0.3, 0.9},
                                                  {0.5, 0.65, 0.15}};
    colororder(newcolors);

    plot(vector_2d{{1, 3}, {2, 4}});
    hold(on);
    auto p2 = plot(vector_2d{{4, 2}, {3, 1}});
    p2[0]->use_y2(true);
    p2[1]->use_y2(true);

    show();
    return 0;
}