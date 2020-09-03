#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto f = [](double x, double y) {
        return exp(-pow(x / 3, 2) - pow(y / 3, 2)) +
               exp(-pow(x + 2, 2) - pow(y + 2, 2));
    };
    fcontour(f)->line_width(1).line_style("--").levels({1, 0.9, 0.8, 0.2, 0.1});

    show();
    return 0;
}