#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 2);
    nexttile();
    plot(rand(20, 0, 1));
    title("Sample 1");

    nexttile();
    plot(rand(20, 0, 1));
    title("Sample 2");

    nexttile();
    plot(rand(20, 0, 1));
    title("Sample 3");

    nexttile();
    plot(rand(20, 0, 1));
    title("Sample 4");

    sgtitle("Size vs. Distance");

    show();
    return 0;
}