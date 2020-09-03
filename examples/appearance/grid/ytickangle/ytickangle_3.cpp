#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 50, 20);
    auto y = transform(x, [](double x) { return pow(x, 2); });
    stem(x, y);

    std::cout << "ytickangle(): " << ytickangle() << std::endl;

    show();
    return 0;
}