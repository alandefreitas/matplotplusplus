#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));
    xticks({0, 4, 6, 10});
    xticks(automatic);

    show();
    return 0;
}