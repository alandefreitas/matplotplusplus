#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::string> newcolors = {"red", "magenta", "blue"};
    colororder(newcolors);

    scatter(iota(1, 10), rand(10, 0, 1))->marker_face(true);
    hold(on);
    scatter(iota(1, 10), rand(10, 0, 1))->line_style("*k");
    scatter(iota(1, 10), rand(10, 0, 1))->marker_face(true);
    hold(off);

    show();
    return 0;
}