#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = {17, 33, 33, 17};
    std::vector<double> explode = {0, 1, 1, 0};
    std::vector<std::string> labels = {"East (17%)", "North (33%)",
                                       "South (33%)", "West (17%)"};

    pie(x, explode, labels);

    show();
    return 0;
}