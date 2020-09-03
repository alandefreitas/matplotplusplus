#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    for (size_t i = 0; i < 4; ++i) {
        subplot(2, 2, i);
        std::string equation = "cos(x**" + num2str(i + 1) + ") + " + num2str(i);
        fplot(equation);
        title(equation);
    }
    subplot(2, 2, 1, true);
    fplot("sin(x)");
    title("sin(x)");
    show();
    return 0;
}