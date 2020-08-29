#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto f = figure(true);

    plot(rand(5, 5, 0, 1));
    save("img/lineplot.eps");
    save("img/lineplot.jpg");

    return 0;
}