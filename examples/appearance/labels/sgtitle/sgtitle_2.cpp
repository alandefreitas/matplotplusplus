#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    subplot(2, 1, 0);
    title("First Subplot");
    subplot(2, 1, 1);
    title("Second Subplot");

    sgtitle("Subplot Grid Title", "red");
    gcf()->title_font_size_multiplier(2.0);

    show();
    return 0;
}