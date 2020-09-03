#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));
    yticks({0, 5, 10});
    yticks(automatic);

    show();
    return 0;
}