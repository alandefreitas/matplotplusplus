#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> X(4);
    X[0] = concat(rand(50, 78, 200), rand(50, 65, 91));
    X[1] = concat(std::vector<double>(50, 1), std::vector<double>(50, 0));
    X[2] = concat(rand(50, 122, 140), rand(50, 105, 131));
    X[3] =
        concat(concat(std::vector<double>(25, 3), std::vector<double>(50, 1)),
               std::vector<double>(25, 2));

    auto p = parallelplot(X, X[3]);

    gca()->x_axis().tick_values({1, 2, 3, 4});
    gca()->x_axis().ticklabels({"f_1", "f_2", "f_3", "f_4"});

    p->axis()[1].tick_values({0, 1});
    p->axis()[1].ticklabels({"false", "true"});

    p->axis()[3].tick_values({1, 2, 3});
    p->axis()[3].ticklabels({"low", "medium", "high"});

    show();
    return 0;
}