//
// Created by Alan Freitas on 2020-07-03.
//

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <matplot/util/colors.h>
#include <matplot/util/common.h>
#include <matplot/util/popen.h>
#include <random>
#include <regex>
#include <set>
#include <string>

// Allow the warnings related to the bundled CImg
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4458)
#pragma warning(disable : 4456)
#pragma warning(disable : 4702)
#endif
#include <CImg.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace matplot {
    bool iequals(std::string_view str1, std::string_view str2) {
        if (str1.size() != str2.size()) {
            return false;
        } else {
            for (size_t i = 0; i < str1.size(); ++i) {
                if (toupper(str1[i]) != toupper(str2[i])) {
                    return false;
                }
            }
        }
        return true;
    }

    bool is_true(std::string_view str) {
        return iequals(str, "on") || iequals(str, "true") ||
               iequals(str, "yes");
    }

    bool is_false(std::string_view str) {
        return iequals(str, "off") || iequals(str, "false") ||
               iequals(str, "no");
    }

    std::string run_and_get_output(const std::string &cmd) {
        std::unique_ptr<FILE, decltype(&PCLOSE)> pipe(POPEN(cmd.c_str(), "r"),
                                                      PCLOSE);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        std::array<char, 128> buffer{};
        std::string result;
        while (fgets(buffer.data(), static_cast<int>(buffer.size()),
                     pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    std::string escape(std::string_view label) {
        std::string escaped;
        escaped.reserve(label.size());

        std::regex_replace(std::back_inserter(escaped), label.begin(), label.end(), std::regex("\""), "\\\"");

        return escaped;
    }

    std::vector<double> linspace(double d1, double d2, size_t n) {
        std::vector<double> x(n);
        // avoid division by zero
        double step = n > 1? (d2 - d1) / static_cast<double>(n - 1) : 0.;
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] = d1 + static_cast<double>(i) * step;
        }
        return x;
    }

    std::vector<double> linspace(double d1, double d2) {
        return linspace(d1, d2, 100);
    }

    std::vector<double> logspace(double d1, double d2, size_t n) {
        return transform(linspace(d1, d2, n),
                         [](double x) { return pow(10, x); });
    }

    std::vector<double> logspace(double d1, double d2) {
        return logspace(d1, d2, 50);
    }

    std::vector<double> iota(double d1, double step, double d2) {
        vector_1d r;
        r.reserve(static_cast<size_t>((d2 - d1) / step));
        for (double i = d1; i <= d2; i += step) {
            r.emplace_back(i);
        }
        return r;
    }

    std::vector<double> iota(double d1, double d2) { return iota(d1, 1., d2); }

    std::vector<double> transform(const std::vector<double> &x,
                                  std::function<double(double)> fn) {
        std::vector<double> y(x.size());
        for (size_t i = 0; i < y.size(); ++i) {
            y[i] = fn(x[i]);
        }
        return y;
    }

    double truncate(double x, double lower_bound, double upper_bound) {
        if (x <= lower_bound) {
            return lower_bound;
        } else if (x >= upper_bound) {
            return upper_bound;
        } else {
            return x;
        }
    }

    std::vector<double> transform(const std::vector<double> &x,
                                  const std::vector<double> &y,
                                  std::function<double(double, double)> fn) {
        std::vector<double> z(x.size());
        size_t n = std::min(x.size(), y.size());
        for (size_t i = 0; i < n; ++i) {
            z[i] = fn(x[i], y[i]);
        }
        return z;
    }

    vector_2d transform(const vector_2d &x, std::function<double(double)> fn) {
        const size_t n_rows = x.size();
        const size_t n_cols = x[0].size();
        vector_2d z(n_rows, vector_1d(n_cols, 0.));
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols; ++j) {
                z[i][j] = fn(x[i][j]);
            }
        }
        return z;
    }

    vector_2d transform(const vector_2d &x, const vector_2d &y,
                        std::function<double(double, double)> fn) {
        const size_t n_rows = std::min(x.size(), y.size());
        const size_t n_cols = std::min(x[0].size(), y[0].size());
        vector_2d z(n_rows, vector_1d(n_cols, 0.));
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols; ++j) {
                z[i][j] = fn(x[i][j], y[i][j]);
            }
        }
        return z;
    }

    vector_1d flatten(const vector_2d &x) {
        vector_1d x_line;
        x_line.reserve(x.size() * x[0].size());
        for (size_t i = 0; i < x.size(); ++i) {
            x_line.insert(x_line.end(), x[i].begin(), x[i].end());
        }
        return x_line;
    }

    double max(const std::vector<double> &x) {
        if (x.empty()) {
            return -std::numeric_limits<double>::max();
        } else {
            return *std::max_element(x.begin(), x.end());
        }
    }

    double min(const std::vector<double> &x) {
        if (x.empty()) {
            return std::numeric_limits<double>::max();
        } else {
            return *std::min_element(x.begin(), x.end());
        }
    }

    double min(const std::vector<std::vector<double>> &x) {
        double m = min(x[0]);
        for (size_t i = 1; i < x.size(); ++i) {
            m = min(m, min(x[i]));
        }
        return m;
    }

    double max(const std::vector<std::vector<double>> &x) {
        double m = max(x[0]);
        for (size_t i = 1; i < x.size(); ++i) {
            m = max(m, max(x[i]));
        }
        return m;
    }

    std::pair<double, double> minmax(const std::vector<double> &x) {
        if (x.empty()) {
            return std::make_pair(min(x), max(x));
        } else {
            auto [min_it, max_it] = std::minmax_element(x.begin(), x.end());
            return std::make_pair(*min_it, *max_it);
        }
    }

    double mean(const std::vector<double> &x) {
        double sum = 0;
        for (const double &xi : x) {
            sum += xi / static_cast<double>(x.size());
        }
        return sum;
    }

    double stddev(const std::vector<double> &x) {
        double m = mean(x);
        double sum = 0;
        for (const double &xi : x) {
            sum += pow(xi - m, 2);
        }
        return sqrt(sum / static_cast<double>(x.size() - 1));
    }

//	windows.h library defines a function-like macro with the name of max()
#ifdef max
//	store the predefined macro in a new one
#define _TEMP_MACRO_ max
//	undefine the problamatic macro.
#undef max
#endif
    void wait() {
#ifdef MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES
        return;
#endif
        // http://www.cplusplus.com/forum/beginner/106769/#msg578325
        // Clear failed/error states of the stream if they are set
        std::cin.clear();

        // If there are any characters in the input buffer
        if (std::cin.rdbuf()->in_avail()) {
            // Throw them away
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Press ENTER to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
#ifdef _Temp_MACRO_
// restore the max() macro.
#define max _TEMP_MACRO_
// undefine the temporary macro.
#undef _TEMP_MACRO_
#endif

    namespace detail {
        static std::mt19937 &default_generator() {
            static std::mt19937 g((std::random_device())());
            return g;
        }
    } // namespace detail

    double randn(double mean, double stddev) {
        std::normal_distribution<double> d(mean, stddev);
        return d(detail::default_generator());
    }

    std::vector<double> randn(size_t n, double mean, double stddev) {
        std::vector<double> x(n);
        std::generate(x.begin(), x.end(), [&] { return randn(mean, stddev); });
        return x;
    }

    double rande(double lambda, double multiplier) {
        std::exponential_distribution<double> d(lambda);
        return d(detail::default_generator()) * multiplier;
    }

    std::vector<double> rande(size_t n, double lambda, double multiplier) {
        std::vector<double> x(n);
        std::generate(x.begin(), x.end(),
                      [&] { return rande(lambda, multiplier); });
        return x;
    }

    double randp(double scale, double shape) {
        std::exponential_distribution<double> d(shape);
        return scale * exp(d(detail::default_generator()));
    }

    std::vector<double> randp(size_t n, double scale, double shape) {
        std::vector<double> x(n);
        std::generate(x.begin(), x.end(), [&] { return randp(scale, shape); });
        return x;
    }

    int randi(int imin, int imax) {
        std::uniform_int_distribution<int> d(imin, imax);
        return d(detail::default_generator());
    }

    std::vector<int> randi(size_t n, int imin, int imax) {
        std::vector<int> x(n);
        std::generate(x.begin(), x.end(), [&] { return randi(imin, imax); });
        return x;
    }

    double rand(double lower_bound, double upper_bound) {
        std::uniform_real_distribution<double> d(lower_bound, upper_bound);
        return d(detail::default_generator());
    }

    std::vector<double> rand(size_t n, double lower_bound, double upper_bound) {
        std::vector<double> x(n);
        std::generate(x.begin(), x.end(),
                      [&] { return rand(lower_bound, upper_bound); });
        return x;
    }

    vector_2d rand(size_t rows, size_t cols, double lower_bound,
                   double upper_bound) {
        vector_2d x(rows);
        for (size_t i = 0; i < x.size(); ++i) {
            x[i] = rand(cols, lower_bound, upper_bound);
        }
        return x;
    }

    std::vector<double> unique(const std::vector<double> &x) {
        std::set<double> s;
        for (const auto &v : x) {
            s.emplace(v);
        }
        std::vector<double> u(s.begin(), s.end());
        return u;
    }

    std::vector<double> reshape(const std::vector<std::vector<double>> &X) {
        std::vector<double> x;
        for (const auto &vec : X) {
            for (const auto &item : vec) {
                x.emplace_back(item);
            }
        }
        return x;
    }

    std::vector<double> concat(const std::vector<double> &a,
                               const std::vector<double> &b) {
        std::vector<double> c(a.begin(), a.end());
        c.insert(c.end(), b.begin(), b.end());
        return c;
    }

    std::string fileread(const std::string &filename) {
        std::ifstream t(filename);
        if (!t) {
            throw std::runtime_error("Cannot open the file " + filename);
        }
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        return str;
    }

    double cosd(double degrees) {
        // 0 -> 0
        // 180 -> pi
        // 360 -> 2pi
        return cos(pi * degrees / 180);
    }

    double sind(double degrees) {
        // 0 -> 0
        // 180 -> pi
        // 360 -> 2pi
        return sin(pi * degrees / 180);
    }

    std::vector<double> deg2rad(const std::vector<double> &degrees) {
        std::vector<double> rad(degrees.size());
        for (size_t i = 0; i < degrees.size(); ++i) {
            rad[i] = pi * degrees[i] / 180;
        }
        return rad;
    }

    double deg2rad(double rho) { return rho * 180. / pi; }

    double round_polar_max(double polar_max) {
        // we need to round the polar max because gnuplot
        // does not use the max value for the r-axis.
        // it uses a round value. If we explicitly set the
        // max value, the x-axis and y-axis get into conflict
        // and things get out of scale.
        // So the only possible solution is to foresee what
        // value gnuplot will set for the r-axis limit.
        double decimals = floor(log10(polar_max));
        double base = polar_max / pow(10, decimals);
        if (base > 1. && base < 1.5) {
            base = 1.5;
        } else {
            base = ceil(base);
            if (base == 7.) {
                base += 1.0;
            } else if (base == 9 || base == 10) {
                base = 1.0;
                decimals++;
            }
        }
        return base * pow(10, decimals);
    }

    double to_positive_radian(double radian) {
        radian = std::fmod(radian, pi * 2.);
        while (radian < 0) {
            // pretty sure this comparison is valid for doubles and floats
            radian += pi * 2.;
        }
        return radian;
    }

    double vector_radians(double x, double y) {
        return to_positive_radian(atan2(y, x));
    }

    double vector_magnitude(double x, double y) {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    double vector_radians(std::complex<double> v) {
        return vector_radians(v.real(), v.imag());
    }

    double vector_magnitude(std::complex<double> v) {
        return vector_magnitude(v.real(), v.imag());
    }

    std::pair<double, double> elliptic2cart(double theta, double rho_w,
                                            double rho_h) {
        return std::make_pair(rho_w * cos(theta), rho_h * sin(theta));
    }

    std::pair<vector_1d, vector_1d>
    elliptic2cart(vector_1d theta, vector_1d rho_w, vector_1d rho_h) {
        std::pair<vector_1d, vector_1d> r;
        r.first.resize(theta.size());
        r.second.resize(theta.size());
        for (size_t i = 0; i < theta.size(); ++i) {
            std::tie(r.first[i], r.second[i]) =
                elliptic2cart(theta[i], rho_w[i], rho_h[i]);
        }
        return r;
    }

    std::pair<vector_1d, vector_1d> elliptic2cart(vector_1d theta, double rho_w,
                                                  double rho_h) {
        std::pair<vector_1d, vector_1d> r;
        r.first.resize(theta.size());
        r.second.resize(theta.size());
        for (size_t i = 0; i < theta.size(); ++i) {
            std::tie(r.first[i], r.second[i]) =
                elliptic2cart(theta[i], rho_w, rho_h);
        }
        return r;
    }

    std::pair<double, double> pol2cart(double theta, double rho) {
        return std::make_pair(rho * cos(theta), rho * sin(theta));
    }

    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, vector_1d rho) {
        return std::make_pair(
            transform(
                theta, rho,
                [](double theta, double rho) { return rho * cos(theta); }),
            transform(theta, rho, [](double theta, double rho) {
                return rho * sin(theta);
            }));
    }

    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, double rho) {
        return std::make_pair(
            transform(theta, [&](double theta) { return rho * cos(theta); }),
            transform(theta, [&](double theta) { return rho * sin(theta); }));
    }

    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x,
                                             const vector_1d &y) {
        vector_2d X(y.size(), std::vector<double>(x.size(), 0));
        vector_2d Y(y.size(), std::vector<double>(x.size(), 0));
        for (size_t i = 0; i < y.size(); ++i) {
            for (size_t j = 0; j < x.size(); ++j) {
                X[i][j] = x[j];
                Y[i][j] = y[i];
            }
        }
        return std::make_pair(X, Y);
    }

    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x_and_y) {
        return meshgrid(x_and_y, x_and_y);
    }

    vector_1d gradient(const vector_1d &z, double spacing) {
        const size_t n = z.size();
        vector_1d d(n);
        d[0] = (z[1] - z[0]) / spacing;
        d[n - 1] = (z[n - 1] - z[n - 2]) / spacing;
        for (size_t i = 1; i < n - 1; ++i) {
            d[i] = ((z[i] - z[i - 1]) + (z[i + 1] - z[i])) / spacing / 2.;
        }
        return d;
    }

    std::pair<vector_2d, vector_2d>
    gradient(const vector_2d &z, double spacing_x, double spacing_y) {
        const size_t n1 = z.size();
        const size_t n2 = z[0].size();
        vector_2d dx(n1, vector_1d(n2, 0.));
        vector_2d dy(n1, vector_1d(n2, 0.));
        for (size_t row = 0; row < n1; ++row) {
            dx[row][0] = (z[row][1] - z[row][0]) / spacing_x;
            dx[row][n2 - 1] = (z[row][n2 - 1] - z[row][n2 - 2]) / spacing_x;
            for (size_t col = 1; col < n2 - 1; ++col) {
                dx[row][col] = ((z[row][col] - z[row][col - 1]) +
                                (z[row][col + 1] - z[row][col])) /
                               spacing_x / 2.;
            }
        }
        for (size_t col = 0; col < n2; ++col) {
            dy[0][col] = (z[1][col] - z[0][col]) / spacing_y;
            dy[n1 - 1][col] = (z[n1 - 1][col] - z[n1 - 2][col]) / spacing_y;
            for (size_t row = 1; row < n1 - 1; ++row) {
                dy[row][col] = ((z[row][col] - z[row - 1][col]) +
                                (z[row + 1][col] - z[row][col])) /
                               spacing_y / 2.;
            }
        }
        return std::make_pair(dx, dy);
    }

    std::pair<vector_2d, vector_2d> gradient(const vector_2d &z,
                                             double spacing) {
        return gradient(z, spacing, spacing);
    }

    double peaks(double x, double y) {
        return 3. * pow(1. - x, 2.) * exp(-pow(x, 2.) - pow(y + 1., 2.)) -
               10. * (x / 5. - pow(x, 3.) - pow(y, 5.)) *
                   exp(-pow(x, 2.) - pow(y, 2.)) -
               1. / 3. * exp(-pow(x + 1., 2.) - pow(y, 2.));
    }

    vector_2d peaks(const vector_2d &X, const vector_2d &Y) {
        const size_t n_rows = std::min(X.size(), Y.size());
        const size_t n_cols = std::min(X[0].size(), Y[0].size());
        vector_2d Z(n_rows, vector_1d(n_cols, 0.));
        for (size_t i = 0; i < n_rows; ++i) {
            for (size_t j = 0; j < n_cols; ++j) {
                Z[i][j] = peaks(X[i][j], Y[i][j]);
            }
        }
        return Z;
    }

    std::tuple<vector_2d, vector_2d, vector_2d> peaks(size_t N) {
        std::vector<double> x = linspace(-3., +3., N);
        std::vector<double> y = linspace(-3., +3., N);
        auto [X, Y] = meshgrid(x, y);
        return std::make_tuple(X, Y, peaks(X, Y));
    }

    vector_2d zeros(size_t rows, size_t cols) {
        return vector_2d(rows, vector_1d(cols, 0.));
    }

    vector_2d ones(size_t rows, size_t cols) {
        return vector_2d(rows, vector_1d(cols, 0.));
    }

    image_channels_t
    cimg2channels(const cimg_library::CImg<unsigned char> &cimg_image) {
        const int n_channels = cimg_image.spectrum();
        const size_t height = cimg_image.height();
        const size_t width = cimg_image.width();
        image_channels_t img(n_channels,
                             image_channel_t(height, image_row_t(width, 0)));
        for (int channel = 0; channel < n_channels; ++channel) {
            for (int i = 0; i < cimg_image.height(); ++i) {
                for (int j = 0; j < cimg_image.width(); ++j) {
                    img[channel][i][j] =
                        cimg_image.operator()(j, i, 0, channel);
                }
            }
        }
        return img;
    }

    cimg_library::CImg<unsigned char> channels2cimg(const image_channels_t &A) {
        cimg_library::CImg<unsigned char> cimg_image(
            static_cast<unsigned int>(A[0].size()),
            static_cast<unsigned int>(A[0][0].size()), 1U,
            static_cast<unsigned int>(A.size()));
        const int n_channels = static_cast<int>(A.size());
        // const size_t height = A[0].size();
        // const size_t width = A[0][0].size();
        for (int channel = 0; channel < n_channels; ++channel) {
            for (int i = 0; i < cimg_image.height(); ++i) {
                for (int j = 0; j < cimg_image.width(); ++j) {
                    cimg_image.operator()(j, i, 0, channel) = A[channel][i][j];
                }
            }
        }
        return cimg_image;
    }

    image_channels_t imread(const std::string &filename) {
        try {
            cimg_library::CImg<unsigned char> cimg_image(filename.c_str());
            return cimg2channels(cimg_image);
        } catch (...) {
            // return empty tuple if we can't open the file
            return image_channels_t{0};
        }
    }

    image_channels_t imresize(const image_channels_t &A, size_t height,
                              size_t width, image_interpolation m) {
        auto cimg_image = channels2cimg(A);
        int interpolation_type = 0;
        switch (m) {
        case image_interpolation::raw:
            interpolation_type = -1;
            break;
        case image_interpolation::additional_space:
            interpolation_type = 0;
            break;
        case image_interpolation::nearest:
            interpolation_type = 1;
            break;
        case image_interpolation::moving_average:
            interpolation_type = 2;
            break;
        case image_interpolation::bilinear:
            interpolation_type = 3;
            break;
        case image_interpolation::grid:
            interpolation_type = 4;
            break;
        case image_interpolation::bicubic:
            interpolation_type = 5;
            break;
        case image_interpolation::lanczos:
            interpolation_type = 6;
            break;
        }
        cimg_image.resize(static_cast<int>(width), static_cast<int>(height), 1,
                          static_cast<int>(A.size()), interpolation_type);
        return cimg2channels(cimg_image);
    }

    image_channels_t imresize(const image_channels_t &A, double scale,
                              image_interpolation m) {
        if (A.empty()) {
            return image_channels_t{};
        }
        auto [h, w] = size(A[0]);
        return imresize(A, static_cast<size_t>(h * scale), static_cast<size_t>(w * scale), m);
    }

    void imwrite(const image_channels_t &A, const std::string &filename) {
        auto cimg_image = channels2cimg(A);
        cimg_image.save(filename.c_str());
    }

    image_channel_t rgb2gray(const image_channels_t &A) {
        image_channel_t img(A[0].size(), image_row_t(A[0][0].size()));
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A[i].size(); ++j) {
                img[i][j] = (A[0][i][j] + A[1][i][j] + A[2][i][j]) / 3;
            }
        }
        return img;
    }

    image_channels_t
    gray2rgb(const image_channel_t &A,
             const std::vector<std::vector<double>> &colormap) {
        image_channels_t img(
            3, image_channel_t(A.size(), image_row_t(A[0].size())));
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A[i].size(); ++j) {
                color_array c =
                    colormap_interpolation(A[i][j], 0, 255, colormap);
                img[0][i][j] = static_cast<unsigned char>(round(c[1] * 255));
                img[1][i][j] = static_cast<unsigned char>(round(c[2] * 255));
                img[2][i][j] = static_cast<unsigned char>(round(c[3] * 255));
            }
        }
        return img;
    }

    image_channels_t gray2rgb(const image_channel_t &A) {
        static std::vector<std::vector<double>> map = {{0, 0, 0}, {1, 1, 1}};
        return gray2rgb(A, map);
    }

    image_channels_t
    gray2rgb(const image_channels_t &A,
             const std::vector<std::vector<double>> &colormap) {
        return gray2rgb(A[0], colormap);
    }

    image_channels_t gray2rgb(const image_channels_t &A) {
        return gray2rgb(A[0]);
    }

    void imwrite(const image_channel_t &A,
                 const std::vector<std::vector<double>> &colormap,
                 const std::string &filename) {
        imwrite(gray2rgb(A, colormap), filename);
    }

    image_channels_t imvignette(const image_channels_t &A, double min_radius,
                                double exponent) {
        auto image = A;
        if (image.empty()) {
            return A;
        }
        if (image.size() < 2) {
            image = gray2rgb(image);
        }
        if (image.size() < 3) {
            image.emplace_back(image[0]);
        }
        auto [h, w] = size(image[0]);
        if (image.size() < 4) {
            image.emplace_back(image_channel_t(h, image_row_t(w, 255)));
        }
        double center_x = static_cast<double>(w) / 2;
        double center_y = static_cast<double>(h) / 2;
        double radius_sq = pow(min(h / 2, w / 2) * min_radius, 2);
        double max_t1 = pow(static_cast<double>(h) - center_y, 2);
        double max_t2 = pow(static_cast<double>(w) - center_x, 2);
        double max_t_minus_radius = (max_t1 + max_t2) - radius_sq;
        auto &alpha_channel = image[3];
        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < w; ++j) {
                double t1 = pow(static_cast<double>(i) - center_y, 2);
                double t2 = pow(static_cast<double>(j) - center_x, 2);
                if (t1 + t2 > radius_sq) {
                    double norm_dist_from_r =
                        ((t1 + t2) - radius_sq) / max_t_minus_radius;
                    norm_dist_from_r = pow(norm_dist_from_r, exponent);
                    alpha_channel[i][j] = static_cast<
                        std::decay_t<decltype(alpha_channel[i][j])>>(
                        255. * (1. - norm_dist_from_r));
                }
            }
        }
        return image;
    }

    vector_2d transpose(const vector_2d &z) {
        vector_2d z2(z[0].size(), vector_1d(z.size()));
        for (size_t i = 0; i < z.size(); ++i) {
            for (size_t j = 0; j < z[0].size(); ++j) {
                z2[j][i] = z[i][j];
            }
        }
        return z2;
    }

    std::vector<std::string> tokenize(std::string_view text,
                                      std::string_view delimiters) {
        std::vector<std::string> tokens;
        size_t pos = 0;
        while ((pos = text.find_first_not_of(delimiters, pos)) !=
               std::string::npos) {
            size_t end_pos = text.find_first_of(delimiters, pos);
            tokens.emplace_back(text.substr(pos, end_pos - pos));
            pos = end_pos;
        }
        return tokens;
    }

    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(const std::vector<std::string> &tokens,
              const std::vector<std::string> &black_list,
              size_t max_cloud_size) {
        // count the frequency of each token
        const bool bl_sorted =
            std::is_sorted(black_list.begin(), black_list.end());
        std::map<std::string, size_t> word_frequency;
        for (const auto &token : tokens) {
            const bool is_in_black_list =
                bl_sorted ? std::binary_search(black_list.begin(),
                                               black_list.end(), token)
                          : std::find(black_list.begin(), black_list.end(),
                                      token) != black_list.end();
            if (!is_in_black_list) {
                auto it = word_frequency.find(token);
                if (it != word_frequency.end()) {
                    ++word_frequency[token];
                } else {
                    word_frequency[token] = 1;
                }
            }
        }

        // sort tokens by frequency
        std::multimap<size_t, std::string, std::greater<>> frequency_word;
        for (const auto &[token, count] : word_frequency) {
            frequency_word.emplace(count, token);
        }

        // get the max_cloud_size most frequent tokens
        std::vector<std::string> unique_tokens;
        std::vector<size_t> token_count;
        size_t i = 0;
        for (const auto &[count, token] : frequency_word) {
            unique_tokens.emplace_back(token);
            token_count.emplace_back(count);
            ++i;
            if (i > max_cloud_size) {
                break;
            }
        }

        // return tokens and counts
        return std::make_pair(unique_tokens, token_count);
    }

    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(std::string_view text,
              const std::vector<std::string> &black_list,
              std::string_view delimiters, size_t max_cloud_size) {
        auto tokens = tokenize(text, delimiters);
        return wordcount(tokens, black_list, max_cloud_size);
    }

    ticks_results calcticks(double limits_min, double limits_max,
                            bool horizontal, double text_size, bool separateExp,
                            bool log) {
        // Default limit on label length (in characters)
        constexpr size_t defMaxChars = 9;
        // Minimum value of the (upper) limit on label length (characters).
        // Setting this value too small will cause problems. Note: This value
        // does not affect the minimum length of the ticklabels.
        constexpr size_t minChars = 6;
        // Default font size for exponents (assumes that font units are
        // 'points')
        constexpr size_t defExpFontSize = 7;
        // Upper limit on number of ticks returned by CALCTICKS
        constexpr size_t initMaxTicks = 11;
        // Multiplier for textSize for vertical orientation when scale is 'log',
        // to account for labels using exponential notation.
        // constexpr double vertExpScale = 1.3;

        // Maximum number of characters in label string
        // Determines numerical precision displayed by the labels,
        // and also affects the tick spacing for horizontal orientation.
        size_t maxChars = defMaxChars;
        if (maxChars < minChars) {
            maxChars = minChars;
        }

        // Initial calculations
        if (log) {
            std::invalid_argument("Not implemented yet. The library does not "
                                  "need that as it is.");
        }

        // Data range
        double range = limits_max - limits_min;

        // Get eps values for rounding
        double lEps = eps(limits_min);
        double uEps = eps(limits_max);
        double minEps = min(lEps, uEps);

        // Vector of allowed tick counts
        std::vector<double> testTickCounts = iota(2, initMaxTicks);

        // Make a list of rough intervals as a starting point
        std::vector<double> roughInts = transform(
            testTickCounts, [&](double x) { return range / (x - 1); });

        // Vector of 'nice' intervals
        std::vector<double> niceVec = {1, 2, 5, 10};

        // Find nice intervals
        // Normalize rough intervals by their scale
        std::vector<double> decRoughInts =
            transform(roughInts, [](double x) { return floor(log10(x)); });
        std::vector<double> normRoughInts =
            transform(roughInts, decRoughInts,
                      [](double x, double y) { return x / pow(10., y); });

        // Get the distances to nice intervals, pick the shortest
        std::vector<std::vector<double>> deltas(
            niceVec.size(), std::vector<double>(roughInts.size()));
        for (size_t i = 0; i < deltas.size(); ++i) {
            for (size_t j = 0; j < deltas[i].size(); ++j) {
                deltas[i][j] = std::abs(normRoughInts[j] - niceVec[i]);
            }
        }

        // idx of ticks with min distance
        std::vector<size_t> idx(roughInts.size(), 0);
        for (size_t i = 0; i < deltas.size(); ++i) {
            for (size_t j = 0; j < deltas[i].size(); ++j) {
                if (deltas[i][j] < deltas[idx[j]][j]) {
                    idx[j] = i;
                }
            }
        }

        // Get the nice intervals and scores
        std::vector<double> niceInts(roughInts.size());
        for (size_t i = 0; i < niceInts.size(); ++i) {
            niceInts[i] = niceVec[idx[i]] * pow(10, decRoughInts[i]);
        }

        // Remove duplicates
        niceInts = unique(niceInts);

        // Get upper and lower limits, fixed by the list of nice intervals.
        // Round out to make sure we get ticks at the original limits.
        std::vector<double> lLims = transform(
            niceInts, [&](double x) { return floor(limits_min / x) * x; });
        std::vector<double> uLims = transform(
            niceInts, [&](double x) { return ceil(limits_max / x) * x; });

        // Get tick counts using the list of nice intervals and limits
        std::vector<double> nTicks(niceInts.size());
        for (size_t i = 0; i < nTicks.size(); ++i) {
            nTicks[i] =
                floor(1 + (uLims[i] - lLims[i] + 10 * minEps) / niceInts[i]);
        }

        // Shrink nice limits that are outside of original limits
        for (size_t idx2 = 0; idx2 < lLims.size(); ++idx2) {
            if (lLims[idx2] < limits_min - 10 * eps(limits_min)) {
                nTicks[idx2] = nTicks[idx2] - 1;
                lLims[idx2] = lLims[idx2] + niceInts[idx2];
            }
            if (uLims[idx2] > limits_max + 10 * eps(limits_min)) {
                nTicks[idx2] = nTicks[idx2] - 1;
                uLims[idx2] = uLims[idx2] - niceInts[idx2];
            }
        }

        // Set values that are almost exactly the original limits to be the
        // original limit value.
        for (size_t idx2 = 0; idx2 < lLims.size(); ++idx2) {
            if (std::abs(lLims[idx2] - limits_min) < 10 * eps(limits_min)) {
                lLims[idx2] = limits_min;
            }
            if (std::abs(uLims[idx2] - limits_max) < 10 * eps(limits_max)) {
                uLims[idx2] = limits_max;
            }
        }

        // Discard values where the limits are reversed or equal
        size_t lim_idx = 0;
        while (lim_idx < lLims.size()) {
            if (lLims[lim_idx] >= uLims[lim_idx]) {
                lLims.erase(lLims.begin() + lim_idx);
                uLims.erase(uLims.begin() + lim_idx);
                nTicks.erase(nTicks.begin() + lim_idx);
                niceInts.erase(niceInts.begin() + lim_idx);
            } else {
                ++lim_idx;
            }
        }

        // Determine label size for each interval
        // Get the decade span of the limits and the decade of the intervals
        std::vector<double> maxAbs =
            transform(lLims, uLims, [](double x, double y) {
                return std::abs(x) > std::abs(y) ? std::abs(x) : std::abs(y);
            });
        std::vector<double> decMax = transform(
            maxAbs, [](double x) { return floor(log10(x)); }); // - nDec;
        std::vector<double> decInts = transform(
            niceInts, [](double x) { return floor(log10(x)); }); // - nDec;
        // Get the number of characters needed for tick labels for normal
        // notation
        std::vector<double> labelChars =
            transform(decInts, decMax, [&](double x, double y) {
                return max(y + 1, 1.) + ((x < 0) ? (1 - x) : 0.);
            });
        for (size_t i = 0; i < labelChars.size(); ++i) {
            if (labelChars[i] > static_cast<double>(maxChars - 1)) {
                labelChars[i] = static_cast<double>(maxChars - 1);
            }
        }

        // Handle exponential notation
        // Determine whether or not to use exponential notation
        std::vector<bool> isExp(decInts.size());
        if (separateExp) {
            for (size_t i = 0; i < decInts.size(); ++i) {
                // Large numbers:
                isExp[i] =
                    (decMax[i] > 6) || (decMax[i] == 6 && decInts[i] > 0);
                // Small numbers:
                isExp[i] = isExp[i] || (decMax[i] < -3) ||
                           (decMax[i] == -3 && decInts[i] < -5);
            }
        } else {
            for (size_t i = 0; i < decInts.size(); ++i) {
                // Large numbers:
                isExp[i] = decMax[i] > 6 || (decMax[i] == 6 && decInts[i] > 3);
                // Small numbers:
                isExp[i] = isExp[i] || (decMax[i] < -3) ||
                           (decMax[i] == -3 && decInts[i] < -5);
            }
        }

        // Get length of exponential labels depending on style
        std::vector<double> expChars;
        if (separateExp) {
            expChars = transform(decInts, decMax, [&](double x, double y) {
                return 2 +
                       max(0., min(static_cast<double>(maxChars) - 3, y - x));
            });
        } else {
            expChars = transform(decInts, decMax, [&](double x, double y) {
                return 2 +
                       max(0., min(max(0., static_cast<double>(maxChars) - 7),
                                   y - x)) +
                       4;
            });
        }

        // Select between normal and exponential label lengths
        for (size_t i = 0; i < labelChars.size(); ++i) {
            if (isExp[i]) {
                labelChars[i] = expChars[i];
            }
        }

        // For consistency, always include space for a negative sign, regardless
        // of the sign of the actual limits
        labelChars = transform(labelChars, [](double x) { return x + 1; });
        // Uncomment to not include the negative sign space if it isn't needed
        // labelChars = labelChars + (lLims < 0);
        // Get label size based on textSize, orientation and length of label
        // string
        std::vector<double> labelSize;
        if (horizontal) {
            labelSize = transform(labelChars, [&](double labelChars) {
                return text_size * labelChars;
            });
        } else {
            labelSize = std::vector<double>(labelChars.size(), text_size);
            // labelSize = transform(labelChars, [&](double labelChars) { return
            // text_size; });
        }
        // Choose the best interval
        // Maximum number of ticks without overlapping labels
        std::vector<double> nMax(uLims.size());
        for (size_t i = 0; i < nMax.size(); ++i) {
            nMax[i] =
                floor((uLims[i] - lLims[i] + 10 * minEps) / labelSize[i]) + 1;
        }
        // Modify this value based on initMaxTicks
        std::vector<double> nMaxScore = transform(nMax, [&](double x) {
            return min(x, initMaxTicks + 0.25 * (x - initMaxTicks));
        });
        // Calculate a score based on the number of ticks relative to the
        // maximum.
        std::vector<double> nTickScore =
            transform(nTicks, nMaxScore, [](double x, double y) {
                return 1 - pow((x / y - 0.7), 2);
            });
        // Severe penalty for more than nMax ticks
        std::vector<double> penalty = transform(
            nTicks, nMax, [](double x, double y) { return 4 * x / y; });
        for (size_t i = 0; i < nTicks.size(); ++i) {
            if (nTicks[i] / nMax[i] <= 1.) {
                penalty[i] = 0;
            }
        }
        // Penalty for more than 0.75*nMax but less than nMax
        std::vector<double> penaltyScale =
            transform(nTicks, nMax, [](double x, double y) { return x / y; });
        for (size_t i = 0; i < penaltyScale.size(); ++i) {
            if (penaltyScale[i] > 0.75 && nTicks[i] / nMax[i] <= 1.) {
                penalty[i] = 1 * nTickScore[i] * penaltyScale[i];
            }
        }
        // Test for intervals that divide the limits exactly
        std::vector<double> rangeTest =
            transform(niceInts, [&](double x) { return range / x; });
        std::vector<bool> isInt(rangeTest.size());
        for (size_t i = 0; i < rangeTest.size(); ++i) {
            isInt[i] = std::abs(rangeTest[i] - round(rangeTest[i])) < 1e-6;
        }
        // Test for intervals that land exactly on the endpoints
        std::vector<bool> hitsEnds(lLims.size());
        for (size_t i = 0; i < hitsEnds.size(); ++i) {
            hitsEnds[i] = (std::abs(lLims[i] - limits_min) < 100 * lEps) &&
                          (std::abs(uLims[i] - limits_max) < 100 * uEps);
        }
        // Compute a score using the above tests and the tick score and penalty
        std::vector<double> scores(isInt.size());
        for (size_t i = 0; i < isInt.size(); ++i) {
            scores[i] =
                isInt[i] + 0.75 * hitsEnds[i] + nTickScore[i] - penalty[i];
        }
        // Penalize for too few ticks
        for (size_t i = 0; i < scores.size(); ++i) {
            if ((nMax[i] > 5) && (nTicks[i] < 4)) {
                scores[i] = scores[i] - 0.5 * scores[i];
            }
        }
        for (size_t i = 0; i < scores.size(); ++i) {
            if ((nMax[i] > 4) && (nTicks[i] == 2)) {
                scores[i] = scores[i] - 0.5 * scores[i];
            }
        }
        // Find the highest score
        auto max_it = std::max_element(scores.begin(), scores.end());
        double maxScore = *max_it;
        size_t bestIdx = max_it - scores.begin();
        double lLim;
        double uLim;
        double interval;
        if (maxScore > -1) {
            // Use the limits and interval with the best score
            lLim = lLims[bestIdx];
            uLim = uLims[bestIdx];
            interval = niceInts[bestIdx];
        } else {
            // If the best score is too low, just return two ticks at the limits
            lLim = limits_min;
            uLim = limits_max;
            interval = range;
        }
        // Create the vector of ticks, making sure to hit lLims and uLims
        // exactly
        std::vector<double> ticks = iota(lLim, interval, (uLim - interval / 2));
        ticks.emplace_back(uLim);
        // Handle 0 as a special case
        for (size_t i = 0; i < ticks.size(); ++i) {
            if (std::abs(ticks[i]) < 10 * minEps) {
                ticks[i] = 0;
            }
        }
        // Calculate how far the label overhangs (in data units) from both ends
        // (0 <= overhang <= labelSize/2)
        std::vector<double> overhang = {
            max(0., limits_min + labelSize[bestIdx] / 2. - lLim),
            max(0., uLim - limits_max + labelSize[bestIdx] / 2)};
        // Create tick labels
        // Get the decade span of the limits and the decade of the intervals
        double maxAbs_d = max(std::abs(ticks.front()), std::abs(ticks.back()));
        double decMax_d = floor(log10(maxAbs_d));
        double decInt = floor(log10(interval));

        ticks_results result;
        if (isExp[bestIdx] && separateExp) {
            // Exponential using 'nice' notation w/ separate exponent string:
            // Label: '1.23'  scaleStr: 'x 10^34'
            // Determine maximum number of decimals and set formatting string
            int n = static_cast<int>(
                max(1., min(max(0., static_cast<double>(maxChars - 3)),
                            decMax_d - decInt)));
            std::string fStr = "%." + num2str(n) + 'f';
            // Normalize the ticks to the correct scale and create the labels
            std::vector<double> normTicks = transform(
                ticks, [&](double x) { return x / pow(10, decMax_d); });
            std::vector<std::string> tickLabels(normTicks.size());
            for (size_t i = 0; i < tickLabels.size(); ++i) {
                tickLabels[i] = num2str(normTicks[i], fStr);
                rtrim(tickLabels[i]);
                ltrim(tickLabels[i]);
            }
            // Create the scale string
            // Use smaller font for exponent if requested
            std::string fs;
            // Use smaller font for exponents if requested
            constexpr bool smallExp = true;
            if (smallExp) {
                constexpr double expFontSize = defExpFontSize;
                fs = "\\fontsize{" + num2str(expFontSize) + "}";
            }
            std::string scaleStr = "x 10^{" + fs + num2str(decMax_d) + "}";

            result.ticks = ticks;
            result.tickLabels = tickLabels;
            result.scaleStr = scaleStr;
            result.overhang = overhang;
        } else if (isExp[bestIdx]) {
            // Exponential using 'ugly' notation: '-2.34e+301'
            // Determine maximum number of decimals and set formatting string
            int n = static_cast<int>(
                max(0., min(max(0., static_cast<double>(maxChars) - 7),
                            decMax_d - decInt)));
            std::string fStr = "%." + num2str(n) + "e";
            // Create tick labels
            std::vector<std::string> tickLabels(ticks.size());
            for (size_t i = 0; i < tickLabels.size(); ++i) {
                tickLabels[i] = num2str(ticks[i], fStr);
                rtrim(tickLabels[i]);
                ltrim(tickLabels[i]);
            }

            // Output 'e' for the scale string as a flag that we are using
            // exponential notation.
            std::string scaleStr = "e";

            result.ticks = ticks;
            result.tickLabels = tickLabels;
            result.scaleStr = scaleStr;
            result.overhang = overhang;
        } else {
            // Normal (fixed point) notation
            // Determine maximum number of decimals and set formatting string
            int n = static_cast<int>(max(
                0., min(-decInt, max(0., static_cast<double>(maxChars) - 3) -
                                     decMax_d)));
            std::string fStr = "%." + num2str(n) + "f";

            // Create tick labels
            std::vector<std::string> tickLabels(ticks.size());
            for (size_t i = 0; i < tickLabels.size(); ++i) {
                tickLabels[i] = num2str(ticks[i], fStr);
                rtrim(tickLabels[i]);
                ltrim(tickLabels[i]);
            }

            std::string scaleStr = "";
            for (size_t i = 0; i < tickLabels.size(); ++i) {
                if (std::abs(ticks[i]) < 10 * minEps) {
                    tickLabels[i] = "0";
                }
            }

            result.ticks = ticks;
            result.tickLabels = tickLabels;
            result.scaleStr = scaleStr;
            result.overhang = overhang;
        }

        return result;
    }

    double distance(double x1, double y1, double x2, double y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

} // namespace matplot
