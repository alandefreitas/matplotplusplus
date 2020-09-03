#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> x = randn(1000, 0, 1);

    size_t num_bins = 25;
    auto h = hist(x, num_bins);

    auto v_it = h->values().begin();
    auto e_it = h->bin_edges().begin();
    while (v_it != h->values().end()) {
        std::cout << "[" << *e_it << ";" << *std::next(e_it)
                  << "] - count: " << *v_it << std::endl;
        ++v_it;
        ++e_it;
    }

    show();
    return 0;
}