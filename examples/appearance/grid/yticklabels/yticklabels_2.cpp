#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));

    yticks({1, 4, 6, 10});
    yticklabels({"A", "B", "C", "D"});

    show();
    return 0;
}