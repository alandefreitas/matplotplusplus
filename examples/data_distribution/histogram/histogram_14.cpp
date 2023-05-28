#include <iostream>
#include <matplot/matplot.h>
#include <random>

constexpr size_t DEFAULT_BOOTSTRAP_REPLICATES = 1000000;

std::vector<double>
bootstrap(std::function<double()> statistic,
          size_t replicates = DEFAULT_BOOTSTRAP_REPLICATES) {
    std::vector<double> data(replicates);
    std::generate(data.begin(), data.end(), statistic);
    return data;
}

std::vector<double>
bootstrap(std::function<double(std::vector<double>)> statistic,
          std::function<double()> data_source, size_t sample_size,
          size_t replicates = DEFAULT_BOOTSTRAP_REPLICATES) {
    std::vector<double> data(replicates);
    return bootstrap(
        [&]() {
            std::vector<double> data(sample_size);
            std::generate(data.begin(), data.end(), data_source);
            return statistic(data);
        },
        replicates);
}

template <typename NUMBER> double mean(const std::vector<NUMBER> &v) {
    double sum = 0.0;
    for (const auto &item : v) {
        sum += static_cast<double>(item);
    }
    return sum / static_cast<double>(v.size());
}

int main() {
    // Example bootstraping from many distributions.
    // Distributions:
    // https://blog.cloudera.com/blog/2015/12/common-probability-distributions-the-data-scientists-crib-sheet/
    using namespace matplot;

    auto f = figure(true);
    f->width(f->width() * 3);
    f->height(f->height() * 2);
    f->x_position(10);
    f->y_position(10);

    enum histogram::normalization norm = histogram::normalization::probability;
    enum histogram::binning_algorithm alg =
        histogram::binning_algorithm::automatic;
    const size_t n_bins = 200;
    const float hist_alpha = 0.7f;
    std::default_random_engine r;
    std::mt19937 generator(r());

    std::cout << "Averages - Normal" << std::endl;
    subplot(2, 3, 0);
    title("Average - Normal / Gaussian - mean(x)= {∑ x_i}/{n} - x_i = N(0,1)");
    xlim({-4, 4});
    ::matplot::legend({});
    std::normal_distribution<double> d(0, 1);
    std::function<double()> normal_data_source = [&]() { return d(generator); };
    hist(bootstrap(mean<double>, normal_data_source, 1), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, normal_data_source, 2), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("2 samples");
    f->draw();
    hist(bootstrap(mean<double>, normal_data_source, 5), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, normal_data_source, 10), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("10 samples");
    f->draw();
    hist(bootstrap(mean<double>, normal_data_source, 30), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    xlabel("Value");
    ylabel("Frequency");

    std::cout << "Averages - Uniform" << std::endl;
    subplot(2, 3, 1);
    title("Average - Uniform - mean(x)= {∑ x_i}/{n} - x_i = U(-1;+1)");
    xlim({-1, 1});
    ::matplot::legend({});
    std::uniform_real_distribution<double> u(-1.0, 1.0);
    std::function<double()> uniform_data_source = [&]() {
        return u(generator);
    };
    hist(bootstrap(mean<double>, uniform_data_source, 1), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, uniform_data_source, 2), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("2 samples");
    f->draw();
    hist(bootstrap(mean<double>, uniform_data_source, 5), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, uniform_data_source, 10), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("10 samples");
    f->draw();
    hist(bootstrap(mean<double>, uniform_data_source, 30), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    xlabel("Value");
    ylabel("Frequency");

    std::cout << "Sum of squares - Chi-squared distribution" << std::endl;
    subplot(2, 3, 2);
    title("Sum of Squares - Chi-Squared - ∑ (x_i - mean(x))^2");
    xlim({0, 5});
    ::matplot::legend();
    double m = 0;
    auto chi2_data_source = [&]() {
        return pow(normal_data_source() - m, 2.0);
    };
    hist(bootstrap(mean<double>, chi2_data_source, 1), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, chi2_data_source, 2), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("2 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi2_data_source, 5), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi2_data_source, 10), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("10 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi2_data_source, 30), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    // xlim({0,50});
    xlabel("Value");
    ylabel("Frequency");

    std::cout << "Square root of sum of squares (Chi distribution)"
              << std::endl;
    subplot(2, 3, 3);
    title("Square Root of Sum of Squares - Chi - √{∑ (x_i - mean(x))^2}");
    xlim({0, 4});
    ::matplot::legend({});
    auto chi_data_source = [&]() { return sqrt(chi2_data_source()); };
    hist(bootstrap(mean<double>, chi_data_source, 1), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, chi_data_source, 2), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("2 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi_data_source, 5), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi_data_source, 10), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("10 samples");
    f->draw();
    hist(bootstrap(mean<double>, chi_data_source, 30), n_bins)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    xlabel("Value");
    ylabel("Frequency");

    std::cout << "Ratio of scaled sums of squares / variance (F distribution)"
              << std::endl;
    subplot(2, 3, 4);
    title("Variance ratio - F - σ_1 / σ_2");
    ::matplot::legend({});
    m = 0.0;
    auto ratio_ss = [&]() { return chi2_data_source() / chi2_data_source(); };
    xlim({0, 5});
    std::vector<double> edges = linspace(0, 10, n_bins);
    hist(bootstrap(mean<double>, ratio_ss, 1), edges)
        ->bin_limits_min(0)
        .bin_limits_max(5)
        .normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, ratio_ss, 2), edges)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("2 samples");
    f->draw();
    hist(bootstrap(mean<double>, ratio_ss, 5), edges)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, ratio_ss, 10), edges)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("10 samples");
    f->draw();
    hist(bootstrap(mean<double>, ratio_ss, 30), edges)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    xlabel("Value");
    ylabel("Frequency");

    std::cout << "Averages - Bernoulli" << std::endl;
    subplot(2, 3, 5);
    title("Average - Bernoulli - mean(x)= {∑ x_i}/{n} - x_i = B(1/6)");
    xlim({0, 1});
    ::matplot::legend();
    std::bernoulli_distribution b(1. / 6.);
    std::function<double()> bernoulli_data_source = [&]() {
        return static_cast<double>(b(generator));
    };
    hist(bootstrap(mean<double>, bernoulli_data_source, 1), 4)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("1 sample");
    f->draw();
    hold(on);
    hist(bootstrap(mean<double>, bernoulli_data_source, 5), 6)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("5 samples");
    f->draw();
    hist(bootstrap(mean<double>, bernoulli_data_source, 30), 10)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("30 samples");
    f->draw();
    hist(bootstrap(mean<double>, bernoulli_data_source, 300, 100000), 50)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("300 samples");
    f->draw();
    hist(bootstrap(mean<double>, bernoulli_data_source, 3000, 10000), 50)
        ->normalization(norm)
        .algorithm(alg)
        .edge_alpha(1.0)
        .face_alpha(hist_alpha)
        .display_name("3000 samples");
    f->draw();
    xlabel("Value");
    ylabel("Frequency");

    //    save("distributions.png");
    //    save("distributions.pdf");
    //    save("distributions.eps");
    //    save("distributions.gif");
    //    save("distributions.jpg");

    show();
    return 0;
}