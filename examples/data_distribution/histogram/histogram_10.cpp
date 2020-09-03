#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(1000, 0, 1);

    auto h = hist(x);
    h->num_bins(15);
    h->bin_edges(iota(-3, 3));
    h->face_color({0., 0., 0.5, 0.5});
    h->edge_color("r");

    show();
    return 0;
}