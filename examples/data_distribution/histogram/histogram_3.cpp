#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(10000, 0, 1);

    subplot(2, 3, 0);
    auto a = histogram::binning_algorithm::automatic;
    hist(x, a, histogram::normalization::count);
    title("Count (c_i)");

    subplot(2, 3, 1);
    hist(x, a, histogram::normalization::probability);
    title("Probability (c_i/N)");

    subplot(2, 3, 2);
    hist(x, a, histogram::normalization::cummulative_count);
    title("Cummulative count (∑_{j=1}^i c_j)");

    subplot(2, 3, 3);
    hist(x, a, histogram::normalization::count_density);
    title("Count density (c_i/w_i)");

    subplot(2, 3, 4);
    hist(x, a, histogram::normalization::pdf);
    title("PDF (c_i/(N w_i))");

    subplot(2, 3, 5);
    hist(x, a, histogram::normalization::cdf);
    title("CDF (∑_{j=1}^i c_j/N)");

    show();
    return 0;
}