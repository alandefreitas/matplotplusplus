#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f1 = figure();
    auto f2 = figure();
    plot(vector_1d{1., 2., 3.}, vector_1d{2., 4., 6.});

    figure(f1);
    scatter(iota(1, 20), rand(20, 0, 1));

    show();
    return 0;
}