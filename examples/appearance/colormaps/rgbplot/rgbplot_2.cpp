#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto r = iota(0, .1, .9);
    auto g = transform(r, [](double r) { return pow(r, 1.8); });
    auto b = transform(r, [](double r) { return pow(r, 2.1); });
    vector_2d mymap_channels{r, g, b};
    auto mymap = transpose(mymap_channels);

    rgbplot(mymap);
    hold(on);
    colormap(mymap);
    colorbar().tick_values({});

    show();
    return 0;
}