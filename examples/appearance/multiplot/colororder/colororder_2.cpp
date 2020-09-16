#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::string> newcolors = {"#FF0000", "#FF8800", "#FFFF00",
                                          "#00BB00", "#0000FF", "#5500FF",
                                          "#AA00FF"};
    colororder(newcolors);

    hold(on);
    for (size_t r = 1; r <= 7; ++r) {
        auto x = linspace(0., static_cast<double>(r), 500);
        auto y =
            transform(x, [&](double x) { return sqrt(pow(r, 2) - pow(x, 2)); });
        plot(x, y)->line_width(15);
    }

    show();
    return 0;
}