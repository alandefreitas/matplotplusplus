#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<double> mpg = {
        18,   15,   18,   16, 17, 15,   14, 14,   14, 15, 15,   14,
        15,   14,   24,   22, 18, 21,   27, 26,   25, 24, 25,   26,
        21,   10,   10,   11, 9,  28,   25, 25,   26, 27, 17.5, 16,
        15.5, 14.5, 22,   22, 24, 22.5, 29, 24.5, 29, 33, 20,   18,
        18.5, 17.5, 29.5, 32, 28, 26.5, 20, 13,   19, 19, 16.5, 16.5,
        13,   13,   13,   28, 27, 34,   31, 29,   27, 24, 23,   36,
        37,   31,   38,   36, 36, 36,   34, 38,   32, 38, 25,   38,
        26,   22,   32,   36, 27, 27,   44, 32,   28, 31};

    std::vector<std::string> origin = {
        "USA",     "USA",     "USA",     "USA",   "USA",   "USA",     "USA",
        "USA",     "USA",     "USA",     "USA",   "USA",   "USA",     "USA",
        "Japan",   "USA",     "USA",     "USA",   "Japan", "Germany", "France",
        "Germany", "Sweden",  "Germany", "USA",   "USA",   "USA",     "USA",
        "USA",     "Italy",   "Germany", "USA",   "USA",   "France",  "USA",
        "USA",     "USA",     "USA",     "USA",   "USA",   "USA",     "USA",
        "USA",     "USA",     "Germany", "Japan", "USA",   "USA",     "USA",
        "USA",     "Germany", "Japan",   "Japan", "USA",   "Sweden",  "USA",
        "France",  "Japan",   "Germany", "USA",   "USA",   "USA",     "USA",
        "USA",     "USA",     "USA",     "USA",   "USA",   "USA",     "USA",
        "USA",     "Germany", "Japan",   "Japan", "USA",   "USA",     "Japan",
        "Japan",   "Japan",   "Japan",   "Japan", "Japan", "USA",     "USA",
        "USA",     "USA",     "Japan",   "USA",   "USA",   "USA",     "Germany",
        "USA",     "USA",     "USA"};

    boxplot(mpg, origin);
    xlabel("All Vehicles");
    ylabel("Miles per Gallon (MPG)");
    title("Miles per Gallon for All Vehicles");

    show();
    return 0;
}