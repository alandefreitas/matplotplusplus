#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));
    xticks({1, 4, 6, 10});
    xticklabels({"A", "B", "C", "D"});

    show();
    return 0;
}