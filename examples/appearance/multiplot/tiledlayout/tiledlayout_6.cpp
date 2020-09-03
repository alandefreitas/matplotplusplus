#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);

    nexttile();
    stem(iota(1, 13));

    nexttile();
    bar(std::vector<double>{10, 22, 31, 43, 52});

    show();
    return 0;
}