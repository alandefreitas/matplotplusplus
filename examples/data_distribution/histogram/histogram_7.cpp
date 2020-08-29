#include <matplot/matplot.h>

#include <cmath>
#include <random>
#include <thread>

int main() {
    using namespace matplot;

    std::vector<std::string> categories = {"no",        "no",        "yes", "yes", "yes", "no",  "no",  "no",  "no",
                                           "undecided", "undecided", "yes", "no",  "no",  "no",  "yes", "no",  "yes",
                                           "no",        "yes",       "no",  "no",  "no",  "yes", "yes", "yes", "yes"};
    hist(categories)->bar_width(0.5);

    wait();
    return 0;
}