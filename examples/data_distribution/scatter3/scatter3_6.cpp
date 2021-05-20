#include <matplot/matplot.h>
#include <random>
#include <tuple>

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
generate_data();

int main() {
    using namespace matplot;

    auto [x, y, z] = generate_data();

    std::vector<double> sizes(x.size() / 3, 16);
    std::fill_n(std::back_inserter(sizes), x.size() / 3, 8);
    std::fill_n(std::back_inserter(sizes), x.size() / 3, 2);

    std::vector<double> colors(x.size() / 3, 1);
    std::fill_n(std::back_inserter(colors), x.size() / 3, 2);
    std::fill_n(std::back_inserter(colors), x.size() / 3, 3);

    scatter3(x, y, z, sizes, colors)->marker_face_color({0, .5, .5});

    show();
    return 0;
}

std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
generate_data() {
    using namespace matplot;
    int n = 16;
    auto r = iota(-n, 2., n);
    auto theta = transform(r, [n](double x) { return x / n * pi; });
    auto phi = transform(r, [n](double x) { return x / n * pi / 2.; });
    auto sinphi = transform(phi, [](double x) { return sin(x); });
    auto cosphi = transform(phi, [](double x) { return cos(x); });
    cosphi.front() = 0;
    cosphi.back() = 0;
    auto sintheta = transform(theta, [](double x) { return sin(x); });
    sintheta.front() = 0;
    sintheta.back() = 0;
    auto costheta = transform(theta, [](double x) { return cos(x); });
    std::vector<std::vector<double>> X(17, std::vector<double>(17, 0.));
    std::vector<std::vector<double>> Y(17, std::vector<double>(17, 0.));
    std::vector<std::vector<double>> Z(17, std::vector<double>(17, 0.));
    for (int i = 0; i < n + 1; ++i) {
        for (int j = 0; j < n + 1; ++j) {
            X[i][j] = cosphi[i] * costheta[j];
            Y[i][j] = cosphi[i] * sintheta[j];
            Z[i][j] = sinphi[i];
        }
    }
    auto X1d = reshape(X);
    auto Y1d = reshape(Y);
    auto Z1d = reshape(Z);
    std::vector<double> x =
        concat(concat(transform(X1d, [](double x) { return x * 0.5; }),
                      transform(X1d, [](double x) { return x * 0.75; })),
               X1d);
    std::vector<double> y =
        concat(concat(transform(Y1d, [](double y) { return y * 0.5; }),
                      transform(Y1d, [](double y) { return y * 0.75; })),
               Y1d);
    std::vector<double> z =
        concat(concat(transform(Z1d, [](double z) { return z * 0.5; }),
                      transform(Z1d, [](double z) { return z * 0.75; })),
               Z1d);

    return std::make_tuple(x, y, z);
}