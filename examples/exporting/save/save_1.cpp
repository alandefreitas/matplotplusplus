#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {29, 17, 14, 13, 12, 4, 11};
    bar(x);

    save("img/barchart.svg");
    save("img/barchart.eps");
    save("img/barchart.gif");
    save("img/barchart.jpg");
    save("img/barchart.html");
    save("img/barchart.txt");
    save("img/barchart.tex");

    return 0;
}