#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contourf(X, Y, Z, {2});

    show();
    return 0;
}