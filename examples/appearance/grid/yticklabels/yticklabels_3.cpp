#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    stem(iota(1, 10));

    yticks({1, 4, 6, 10});
    yticklabels({"A", "B", "C", "D"});

    yticks(automatic);
    yticklabels(automatic);

    show();
    return 0;
}