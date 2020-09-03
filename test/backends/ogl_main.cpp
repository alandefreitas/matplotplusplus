#include <matplot/backend/opengl.h>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    // Create figure with backend
    auto f = figure<backend::opengl>(true);
    auto ax = f->current_axes();
    ax->xlim({0., 2. * pi});
    ax->ylim({-1.5, 1.5});
    ax->yticks(iota(-1.5, 0.5, +1.5));
    ax->xticks(iota(0., 1., 2. * pi));

    // Another figure
    auto f2 = figure<backend::opengl>(true);
    auto ax2 = f2->add_subplot(2, 1, 0);
    auto ax3 = f2->add_subplot(2, 1, 1);

    // Start rendering
    while (!f->should_close() && !f2->should_close()) {
        // Create plots
        double seconds = backend::opengl::get_time();
        std::vector<double> x = linspace(0., 2. * pi);
        std::vector<double> y =
            transform(x, [&](auto x) { return sin(x + seconds); });
        ax->hold(off);
        ax->plot(x, y, "-o");
        ax->hold(on);
        const vector_1d minus_y = transform(y, [](auto y) { return -y; });
        ax->plot(x, minus_y, "--xr");
        ax->plot(x, transform(x, [](auto x) { return x / pi - 1.; }), "-:gs");
        ax->plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

        // Only one line in figure 2
        ax2->plot(x, y, "-o");
        ax3->plot(x, minus_y, "--xr");

        // Draw the figures
        f->draw();
        f2->draw();
    }

    return 0;
}