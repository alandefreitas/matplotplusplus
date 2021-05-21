#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_1d data = {2, 4, 6, 7, 8, 7, 5, 2};
    stem(data);

    auto r1 = rectangle(2.5, 5.5, 2, 2);
    r1->color("red");

    auto r2 = rectangle(6.5, 4.5, 1, 1);
    r2->fill(true);
    r2->color({0.8f, 0.f, 0.f, 1.f});

    show();
    return 0;
}