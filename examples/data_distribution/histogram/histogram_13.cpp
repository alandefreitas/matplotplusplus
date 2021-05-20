#include <cmath>
#include <matplot/matplot.h>
#include <random>
#include <thread>

template <class T> void cout_vector_summary(const std::vector<T> &x);

int main() {
    using namespace matplot;

    {
        std::vector<double> x = randn(100, 0, 1);
        cout_vector_summary(x);
        auto [N, edges] = histcounts(x);
        for (size_t i = 0; i < N.size(); ++i) {
            std::cout << "[" << edges[i] << ";" << edges[i + 1] << "]: " << N[i]
                      << std::endl;
        }
    }

    {
        std::vector<double> x = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        cout_vector_summary(x);
        auto [N, edges] = histcounts(x, 6);
        for (size_t i = 0; i < N.size(); ++i) {
            std::cout << "[" << edges[i] << ";" << edges[i + 1] << "]: " << N[i]
                      << std::endl;
        }
    }

    {
        std::vector<double> x = randn(100, 0, 1);
        cout_vector_summary(x);
        std::vector<double> edges = {-5, -4, -2, -1, -0.5, 0, 0.5, 1, 2, 4, 5};
        auto N = histcounts(x, edges);
        for (size_t i = 0; i < N.size(); ++i) {
            std::cout << "[" << edges[i] << ";" << edges[i + 1] << "]: " << N[i]
                      << std::endl;
        }
    }

    {
        std::vector<double> x = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                 29, 31, 37, 41, 43, 47, 53, 59, 61,
                                 67, 71, 73, 79, 83, 89, 97};
        cout_vector_summary(x);
        auto [N, edges] = histcounts(x, histogram::normalization::probability);
        for (size_t i = 0; i < N.size(); ++i) {
            std::cout << "[" << edges[i] << ";" << edges[i + 1] << "]: " << N[i]
                      << std::endl;
        }
    }

    {
        std::vector<int> xi = randi(100, -5, 5);
        cout_vector_summary(xi);
        std::vector<double> x(xi.size());
        std::copy(xi.begin(), xi.end(), x.begin());
        auto [N, edges] = histcounts(x, histogram::binning_algorithm::integers);
        for (size_t i = 0; i < N.size(); ++i) {
            std::cout << "[" << edges[i] << ";" << edges[i + 1] << "]: " << N[i]
                      << std::endl;
        }
    }

    return 0;
}

template <class T> void cout_vector_summary(const std::vector<T> &x) {
    std::cout << "x: [";
    for (size_t i = 0; i < std::min(x.size(), size_t(10)); ++i) {
        if (i != 0) {
            std::cout << " ";
        }
        std::cout << x[i];
    }
    if (x.size() > 10) {
        std::cout << "...";
    }
    std::cout << "]" << std::endl;
}
