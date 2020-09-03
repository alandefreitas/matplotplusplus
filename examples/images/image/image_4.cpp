#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::tuple<vector_2d, vector_2d, vector_2d> C;
    auto &[r, g, b] = C;
    r = transform(vector_2d{{.1, .2, .3}, {.4, .5, .6}, {.7, .8, .9}},
                  [](double x) { return x * 255; });
    g = zeros(3, 3);
    b = zeros(3, 3);

    image(C);

    show();
    return 0;
}