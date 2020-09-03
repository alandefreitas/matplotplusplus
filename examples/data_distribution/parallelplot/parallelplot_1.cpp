#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X(3);
    X[0] = concat(rand(50, 78, 100), rand(50, 65, 91));
    X[1] = concat(std::vector<double>(50, 1), std::vector<double>(50, 0));
    X[2] = concat(rand(50, 122, 140), rand(50, 105, 131));

    auto p = parallelplot(X);

    gca()->x_axis().tick_values({1, 2, 3});
    gca()->x_axis().ticklabels({"f_1", "f_2", "f_3"});

    p->axis()[1].tick_values({0, 1});
    p->axis()[1].ticklabels({"false", "true"});

    show();
    return 0;
}