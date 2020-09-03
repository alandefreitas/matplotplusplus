#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    auto x = linspace(0, 3 * pi, 200);
    auto y = transform(x, [&](double x) { return cos(x) + rand(0, 1); });
    auto c = linspace(1, 10, x.size());

    scatter(x, y, std::vector<double>{}, c);

    show();
    return 0;
}