#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    plot(rand(5, 5, 0, 1));
    xticklabels({});

    wait();
    return 0;
}