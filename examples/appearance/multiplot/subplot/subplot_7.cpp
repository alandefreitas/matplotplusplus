#include <iostream>
#include <matplot/matplot.h>
#include <set>
#include <thread>
#include <vector>

int main() {
    using namespace matplot;
    std::vector<axes_handle> ax;
    for (size_t i = 0; i < 4; ++i) {
        ax.emplace_back(subplot(2, 2, i));
    }

    subplot(ax[1]);
    auto p = fplot("sin(x)");
    p->line_spec().color({0.1f, 0.5f, 0.1f});
    p->line_spec().line_width(2);
    title("Second subplot");
    axis({0, 50, -1, 1});

    show();
    return 0;
}