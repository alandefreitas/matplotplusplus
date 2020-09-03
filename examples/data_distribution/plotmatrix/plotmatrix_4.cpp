#include <matplot/matplot.h>
#include <random>
#include <tuple>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X = {randn(100, 0, 1), randn(100, 0, 1),
                                          randn(100, 0, 1)};

    auto [S, H, axs] = plotmatrix(X);
    S[2][0]->color("g");
    S[2][0]->marker("*");
    H[2]->edge_color("k");
    H[2]->face_color("g");
    axs[0][1]->title("A Comparison of Data Sets");

    show();
    return 0;
}