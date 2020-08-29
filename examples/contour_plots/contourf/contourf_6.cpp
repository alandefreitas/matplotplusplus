#include <matplot/matplot.h>
#include <thread>

int main() {
    using namespace matplot;

    auto [X, Y, Z] = peaks();
    contourf(X, Y, Z)->line_width(3);

    wait();
    return 0;
}