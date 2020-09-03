#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(5000, 5, 2);

    auto h = hist(x);
    h->normalization(histogram::normalization::pdf);
    hold(on);

    double mu = 5;
    double sigma = 2;
    auto f = [&](double y) {
        return exp(-pow((y - mu), 2.) / (2. * pow(sigma, 2.))) /
               (sigma * sqrt(2. * pi));
    };
    fplot(f, std::array<double, 2>{-5, 15})->line_width(1.5);

    show();
    return 0;
}