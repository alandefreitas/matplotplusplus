#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(10000, 0, 1);

    subplot(2, 3, 0);
    hist(x, histogram::binning_algorithm::automatic);
    title("Automatic binning");

    subplot(2, 3, 1);
    hist(x, histogram::binning_algorithm::scott);
    title("Scott's rule");

    subplot(2, 3, 2);
    hist(x, histogram::binning_algorithm::fd);
    title("Freedman-Diaconis rule");

    subplot(2, 3, 3);
    hist(x, histogram::binning_algorithm::integers);
    title("Integers rule");

    subplot(2, 3, 4);
    hist(x, histogram::binning_algorithm::sturges);
    title("Sturges' rule");

    subplot(2, 3, 5);
    hist(x, histogram::binning_algorithm::sqrt);
    title("Square root rule");

    save("histogram_2.svg");

    show();
    return 0;
}