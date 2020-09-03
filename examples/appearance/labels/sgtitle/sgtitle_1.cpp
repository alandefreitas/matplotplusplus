#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    subplot(2, 2, 0);
    title("First Subplot");
    subplot(2, 2, 1);
    title("Second Subplot");
    subplot(2, 2, 2);
    title("Third Subplot");
    subplot(2, 2, 3);
    title("Fourth Subplot");

    sgtitle("Subplot Grid Title");

    show();
    return 0;
}