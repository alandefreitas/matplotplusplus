#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));
    xticks({1, 4, 6, 10});
    xticklabels({"A", "B", "C", "D"});
    xticks(automatic);
    xticklabels(automatic);

    show();
    return 0;
}