#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X = {randn(100, 0, 1), randn(100, 0, 1),
                                          randn(100, 0, 1)};

    auto [S, H, axs] = plotmatrix(X);
    (void) H;
    S[2][0]->color("g");
    S[2][0]->marker("*");
    axs[0][1]->title("A Comparison of Data Sets");

    show();
    return 0;
}