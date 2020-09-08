//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_COMMON_H
#define MATPLOTPLUSPLUS_COMMON_H

#include <algorithm>
#include <complex>
#include <functional>
#include <map>
#include <matplot/util/concepts.h>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <cctype>

namespace matplot {
    bool iequals(std::string_view str1, std::string_view str2);
    bool is_true(std::string_view str);
    bool is_false(std::string_view str);
    std::string run_and_get_output(const std::string &command);
    std::string escape(std::string_view label);

    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                    return !std::isspace(ch);
                }));
    }

    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             [](int ch) { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    inline void rtrim(std::string &s, char d) {
        s.erase(
            std::find_if(s.rbegin(), s.rend(), [d](int ch) { return ch != d; })
                .base(),
            s.end());
    }

    template <class T> std::string num2str(Arithmetic<T> num) {
        std::ostringstream ss;
        ss << num;
        return ss.str();
    }

    template <class T>
    std::string num2str(Arithmetic<T> num, const std::string &format) {
        constexpr size_t max_buffer_size = 100;
        char buffer[max_buffer_size];
        int cx = snprintf(buffer, max_buffer_size, format.c_str(), num);
        if (cx >= 0 && cx < max_buffer_size) {
            return std::string(buffer);
        } else {
            return num2str(num);
        }
    }

    template <class T = double> T str2num(std::string_view text) {
        std::istringstream ss((std::string(text)));
        T result;
        return ss >> result ? result : 0;
    }

    using vector_1d = std::vector<double>;

    using vector_2d = std::vector<vector_1d>;

    std::vector<double> linspace(double lower_bound, double upper_bound);

    std::vector<double> linspace(double lower_bound, double upper_bound,
                                 size_t n);

    std::vector<double> logspace(double lower_bound, double upper_bound);

    std::vector<double> logspace(double lower_bound, double upper_bound,
                                 size_t n);

    std::vector<double> iota(double lower_bound, double upper_bound);

    std::vector<double> iota(double lower_bound, double step,
                             double upper_bound);

    vector_1d transform(const vector_1d &x, std::function<double(double)> fn);
    vector_1d transform(const vector_1d &x, const vector_1d &y,
                        std::function<double(double, double)> fn);
    vector_2d transform(const vector_2d &x, std::function<double(double)> fn);
    vector_2d transform(const vector_2d &x, const vector_2d &y,
                        std::function<double(double, double)> fn);

    vector_1d flatten(const vector_2d &x);

    std::vector<double> unique(const std::vector<double> &x);

    double min(const std::vector<double> &x);
    double min(const std::vector<std::vector<double>> &x);
    double max(const std::vector<double> &x);
    double max(const std::vector<std::vector<double>> &x);
    std::pair<double, double> minmax(const std::vector<double> &x);

    template <typename T> T min(T val1, T val2) {
        return val1 < val2 ? val1 : val2;
    }

    template <typename T, typename... Ts> T min(T val1, T val2, Ts &&... vs) {
        return val1 < val2 ? min(val1, std::forward<Ts>(vs)...)
                           : min(val2, std::forward<Ts>(vs)...);
    }

    template <typename T> T max(T val1, T val2) {
        return val1 > val2 ? val1 : val2;
    }

    template <typename T, typename... Ts> T max(T val1, T val2, Ts &&... vs) {
        return val1 > val2 ? max(val1, std::forward<Ts>(vs)...)
                           : max(val2, std::forward<Ts>(vs)...);
    }

    double mean(const std::vector<double> &x);
    double stddev(const std::vector<double> &x);

    void wait();

    constexpr double pi =
        3.141592653589793238462643383279502884197169399375105820974944592307816406286;
    constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

    template <class T> std::vector<T> vectorize(const std::vector<T> &result) {
        return result;
    }

    template <class T> std::vector<T> vectorize(const T &result) {
        return std::vector<T>({result});
    }

    template <size_t N, class T>
    inline std::array<T, N> to_array(std::initializer_list<T> il) {
        std::array<T, N> r;
        auto r_it = r.begin();
        auto il_it = il.begin();
        while (r_it != r.end() && il_it != il.end()) {
            *r_it = *il_it;
            ++r_it;
            ++il_it;
        }
        return r;
    }

    template <size_t N, class T>
    inline std::array<T, N> to_array(const std::vector<T> &v) {
        std::array<T, N> r;
        auto r_it = r.begin();
        auto v_it = v.begin();
        while (r_it != r.end() && v_it != v.end()) {
            *r_it = *v_it;
            ++r_it;
            ++v_it;
        }
        return r;
    }

    namespace detail {
        template <typename T, typename U>
        using forward_or_copy =
            std::conditional_t<std::is_same_v<T, U>, const U &, U>;
    }

    template <class T>
    detail::forward_or_copy<T, vector_1d> to_vector_1d(const T &v) {
        if constexpr (std::is_same_v<T, vector_1d>) {
            return v;
        } else {
            using std::begin, std::end;

            return vector_1d(begin(v), end(v));
        }
    }

    template <class T>
    detail::forward_or_copy<T, vector_2d> to_vector_2d(const T &v) {
        if constexpr (std::is_same_v<T, vector_2d>) {
            return v;
        } else {
            using std::begin, std::end;

            vector_2d r(std::distance(begin(v), end(v)));
            std::transform(
                begin(v), end(v), r.begin(),
                [](auto &&e) -> vector_1d { return to_vector_1d(e); });
            return r;
        }
    }

    template <class T>
    detail::forward_or_copy<T, std::vector<vector_2d>> to_vector_3d(const T &v) {
        if constexpr (std::is_same_v<T, std::vector<vector_2d>>) {
            return v;
        } else {
            using std::begin, std::end;

            std::vector<vector_2d> r(std::distance(begin(v), end(v)));
            std::transform(
                begin(v), end(v), r.begin(),
                [](auto &&e) -> vector_2d { return to_vector_2d(e); });
            return r;
        }
    }

    template <class T> inline T norm(const std::vector<T> &v) {
        T sum = 0.;
        for (const auto &vk : v) {
            sum += pow(vk, 2.);
        }
        return sqrt(sum);
    }

    double randn(double mean, double stddev);
    std::vector<double> randn(size_t n, double mean, double stddev);

    /// Exponential distribution
    double rande(double lambda, double multiplier = 1.0);
    std::vector<double> rande(size_t n, double lambda, double multiplier = 1.0);

    /// Pareto distribution
    double randp(double scale, double shape = 1.0);
    std::vector<double> randp(size_t n, double scale, double shape = 1.0);

    int randi(int imin, int imax);
    std::vector<int> randi(size_t n, int imin, int imax);

    double rand(double lower_bound, double upper_bound);
    vector_1d rand(size_t n, double lower_bound, double upper_bound);
    vector_2d rand(size_t rows, size_t cols, double lower_bound,
                   double upper_bound);

    std::vector<double> reshape(const std::vector<std::vector<double>> &X);

    std::vector<double> concat(const std::vector<double> &a,
                               const std::vector<double> &b);

    template <class... Args>
    std::vector<double> concat(const std::vector<double> &a,
                               const std::vector<double> &b, Args&&... args) {
        std::vector<double> r = concat(a, b);
        return concat(r, std::forward<Args>(args)...);
    }

    double truncate(double x, double lower_bound, double upper_bound);

    template <class Arg1, class TUPLE>
    void reorder_parameter_pack_in_tuple(TUPLE &t, Arg1 x) {
        std::get<Arg1>(t) = x;
    }

    template <class Arg1, class... Args, class TUPLE>
    void reorder_parameter_pack_in_tuple(TUPLE &t, Arg1 x, Args&&... args) {
        std::get<Arg1>(t) = x;
        reorder_parameter_pack_in_tuple(t, std::forward<Args>(args)...);
    }

    std::string fileread(const std::string &filename);

    template <class T, class T2 = T, class FN = std::less<T>>
    void rank_elements(T first, T last, T2 rank_input,
                       FN comp = std::less<T>()) {
        using value_and_rank =
            std::pair<std::remove_reference_t<decltype(*first)> const *,
                      size_t>;
        std::vector<value_and_rank> v;
        size_t r = 0;
        while (first != last) {
            v.emplace_back(&(*first), r);
            ++first;
            ++r;
        }
        std::sort(v.begin(), v.end(), [comp](const auto &a, const auto &b) {
            return comp(*a.first, *b.first);
        });
        for (const auto &[ptr, ranking] : v) {
            *rank_input = ranking;
            ++rank_input;
        }
    }

    /// Cosine of argument in degreescollapse
    double cosd(double degrees);

    /// Sine of argument in degreescollapse
    double sind(double degrees);

    /// Sine of argument in degreescollapse
    std::vector<double> deg2rad(const std::vector<double> &degrees);

    double deg2rad(double rho);

    /// Round up the maximum polar value
    double round_polar_max(double polar_max);

    double to_positive_radian(double radian);

    /// Calculate the radians rho of a vector (u,v)
    double vector_radians(double u, double v);

    /// Calculate the magnitude theta of a vector (u,v)
    double vector_magnitude(double u, double v);

    /// Calculate the radians rho of a complex number u + iv
    double vector_radians(std::complex<double> v);

    /// Calculate the magnitude theta of a complex number u + iv
    double vector_magnitude(std::complex<double> v);

    /// Calculate cartesian coordinates of vector in polar coordinates
    std::pair<double, double> pol2cart(double theta, double rho);
    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, vector_1d rho);
    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, double rho);

    std::pair<double, double> elliptic2cart(double theta, double rho_w,
                                            double rho_h);
    std::pair<vector_1d, vector_1d> elliptic2cart(vector_1d theta, double rho_w,
                                                  double rho_h);
    std::pair<vector_1d, vector_1d>
    elliptic2cart(vector_1d theta, vector_1d rho_w, vector_1d rho_h);

    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x,
                                             const vector_1d &y);
    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x_and_y);

    vector_1d gradient(const vector_1d &z, double spacing = 1.0);
    std::pair<vector_2d, vector_2d> gradient(const vector_2d &z,
                                             double spacing = 1.0);
    std::pair<vector_2d, vector_2d>
    gradient(const vector_2d &z, double spacing_x, double spacing_y);

    double peaks(double x, double y);
    vector_2d peaks(const vector_2d &X, const vector_2d &Y);
    std::tuple<vector_2d, vector_2d, vector_2d> peaks(size_t N = 49);

    template <class T1, class T2> struct pair_hash {
        std::size_t operator()(const std::pair<int, int> &p) const {
            std::size_t h1 = std::hash<T1>()(p.first);
            std::size_t h2 = std::hash<T2>()(p.second);
            return h1 ^ h2;
        }
    };

    template <class T> std::pair<size_t, size_t> size(const std::vector<T> &v) {
        if (v.empty()) {
            return std::make_pair(0, 0);
        } else {
            return std::make_pair(v.size(), v[0].size());
        }
    }

    vector_2d zeros(size_t rows, size_t cols);
    vector_2d ones(size_t rows, size_t cols);

    using image_row_t = std::vector<unsigned char>;
    using image_channel_t = std::vector<image_row_t>;
    // The image might have 1 channel (B&W), 3 channels (RGB), or 4 channels
    // (RGBA)
    using image_channels_t = std::vector<image_channel_t>;

    /// Core imread function
    /// Return grayscale image (vector::size() == 1),
    ///        rgb image (vector::size() == 3),
    ///        rgba image (vector::size() == 4),
    ///        empty image (vector::size() == 0)
    image_channels_t imread(const std::string &filename);

    enum class image_interpolation {
        raw,
        additional_space,
        nearest,
        moving_average,
        bilinear,
        grid,
        bicubic,
        lanczos
    };

    image_channel_t rgb2gray(const image_channels_t &A);
    image_channels_t gray2rgb(const image_channel_t &A,
                              const std::vector<std::vector<double>> &colormap);
    image_channels_t gray2rgb(const image_channel_t &A);
    image_channels_t gray2rgb(const image_channels_t &A,
                              const std::vector<std::vector<double>> &colormap);
    image_channels_t gray2rgb(const image_channels_t &A);

    image_channels_t
    imresize(const image_channels_t &A, double scale,
             image_interpolation m = image_interpolation::bicubic);

    image_channels_t
    imresize(const image_channels_t &A, size_t height, size_t width,
             image_interpolation m = image_interpolation::bicubic);

    void imwrite(const image_channels_t &A, const std::string &filename);

    void imwrite(const image_channel_t &A,
                 const std::vector<std::vector<double>> &colormap,
                 const std::string &filename);

    void imwrite(const image_channels_t &A, const std::string &filename);

    image_channels_t imvignette(const image_channels_t &A,
                                double min_radius = 1., double exponent = 0.5);

    template <class T = double> std::vector<std::vector<T>> eye(size_t n) {
        std::vector<std::vector<T>> r(n, std::vector<T>(n, static_cast<T>(0)));
        for (size_t i = 0; i < n; ++i) {
            r[i][i] = 1.;
        }
        return r;
    }

    vector_2d transpose(const vector_2d &z);

    std::vector<std::string>
    tokenize(std::string_view text, std::string_view delimiters = " ',\n\r\t\".!?:");

    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(const std::vector<std::string> &tokens,
              const std::vector<std::string> &black_list,
              std::string_view delimiters = " ',\n\r\t\".!?:;",
              size_t max_cloud_size = 100);

    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(std::string_view text,
              const std::vector<std::string> &black_list,
              std::string_view delimiters = " ',\n\r\t\".!?:;",
              size_t max_cloud_size = 100);

    // Distance from x to the next larger floating point number
    template <class FLOAT>
    std::enable_if_t<std::is_same_v<FLOAT, float> ||
                         std::is_same_v<FLOAT, double> ||
                         std::is_same_v<FLOAT, long double>,
                     FLOAT>
    eps(FLOAT x = 1.) {
        constexpr long double max_long_double =
            std::numeric_limits<long double>::max();
        // std::nexttoward is more precise than std::nextafter
        if constexpr (std::is_same_v<FLOAT, double>) {
            return std::nexttoward(x, max_long_double) - x;
        } else if constexpr (std::is_same_v<FLOAT, float>) {
            return std::nexttowardf(x, max_long_double) - x;
        } else if constexpr (std::is_same_v<FLOAT, long double>) {
            return std::nexttowardl(x, max_long_double) - x;
        } else {
            throw std::logic_error(
                "eps: FLOAT type needs to be float, double or long double");
        }
    }

    template <class T>
    std::vector<T> repmat(const std::vector<T> &a, size_t n) {
        std::vector<T> b(a.size() * n);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < a.size(); ++j) {
                b[i * a.size() + j] = a[j];
            }
        }
        return b;
    }

    struct ticks_results {
        std::vector<double> ticks;
        std::vector<std::string> tickLabels;
        std::string scaleStr;
        std::vector<double> minorTicks;
        std::vector<double> overhang;
    };

    /// \brief Find ticks at "nice" intervals for an axis
    /// For now, we only use our own ticks when we need to take control
    /// over the gnuplot ticks. Sometimes the gnuplot ticks does not
    /// give us the behaviour we expect. For instance, this is the case
    /// when plotting parallel coordinates.
    /// We could easily adapt class axis to always use this function
    /// instead of the automatic gnuplot ticks but there's no need
    /// for that at the moment.
    /// The algorithm in this function is inspired by this matplot script:
    /// https://www.mathworks.com/matlabcentral/fileexchange/30671-calcticks
    /// We don't implement the version for log scale because, by coincidence,
    /// the library does not need it at this point.
    /// \param limits_min Minimum value
    /// \param limits_max Maximum value
    /// \param horizontal True if axis is horizontal (this changes the room for
    /// labels) \param text_size Size in height or width of a char \param log
    /// True if scale is logarithmic (this will throw an error for now) \return
    /// Nice ticks and their labels in a ticks_results struct \see
    /// https://www.mathworks.com/matlabcentral/fileexchange/30671-calcticks
    ticks_results calcticks(double limits_min = -5, double limits_max = +5,
                            bool horizontal = false, double text_size = 1.25,
                            bool separateExp = true, bool log = false);

    double distance(double x1, double y1, double x2, double y2);

} // namespace matplot

#endif // MATPLOTPLUSPLUS_COMMON_H
