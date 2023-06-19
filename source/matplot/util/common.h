//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_COMMON_H
#define MATPLOTPLUSPLUS_COMMON_H

#include <matplot/detail/config.h>
#include <algorithm>
#include <cctype>
#include <complex>
#include <functional>
#include <map>
#include <matplot/util/concepts.h>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace matplot {
    MATPLOT_EXPORTS
    bool iequals(std::string_view str1, std::string_view str2);
    MATPLOT_EXPORTS
    bool is_true(std::string_view str);
    MATPLOT_EXPORTS
    bool is_false(std::string_view str);
    MATPLOT_EXPORTS
    std::string run_and_get_output(const std::string &command);
    MATPLOT_EXPORTS
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
        ss.precision(10);
        ss << std::fixed;
        ss << num;
        return ss.str();
    }

    template <class T>
    std::string num2str(Arithmetic<T> num, const std::string &format) {
        constexpr int max_buffer_size = 100;
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

    MATPLOT_EXPORTS
    std::vector<double> linspace(double lower_bound, double upper_bound);

    MATPLOT_EXPORTS
    std::vector<double> linspace(double lower_bound, double upper_bound,
                                 size_t n);

    MATPLOT_EXPORTS
    std::vector<double> logspace(double lower_bound, double upper_bound);

    MATPLOT_EXPORTS
    std::vector<double> logspace(double lower_bound, double upper_bound,
                                 size_t n);

    MATPLOT_EXPORTS
    std::vector<double> iota(double lower_bound, double upper_bound);

    MATPLOT_EXPORTS
    std::vector<double> iota(double lower_bound, double step,
                             double upper_bound);

    MATPLOT_EXPORTS
    vector_1d transform(const vector_1d &x, std::function<double(double)> fn);
    MATPLOT_EXPORTS
    vector_1d transform(const vector_1d &x, const vector_1d &y,
                        std::function<double(double, double)> fn);
    MATPLOT_EXPORTS
    vector_2d transform(const vector_2d &x, std::function<double(double)> fn);
    MATPLOT_EXPORTS
    vector_2d transform(const vector_2d &x, const vector_2d &y,
                        std::function<double(double, double)> fn);

    MATPLOT_EXPORTS
    vector_1d flatten(const vector_2d &x);

    MATPLOT_EXPORTS
    std::vector<double> unique(const std::vector<double> &x);

    MATPLOT_EXPORTS
    double min(const std::vector<double> &x);
    MATPLOT_EXPORTS
    double min(const std::vector<std::vector<double>> &x);
    MATPLOT_EXPORTS
    double max(const std::vector<double> &x);
    MATPLOT_EXPORTS
    double max(const std::vector<std::vector<double>> &x);
    MATPLOT_EXPORTS
    std::pair<double, double> minmax(const std::vector<double> &x);

    template <typename T> T min(T val1, T val2) {
        return val1 < val2 ? val1 : val2;
    }

    template <typename T, typename... Ts> T min(T val1, T val2, Ts &&...vs) {
        return val1 < val2 ? min(val1, std::forward<Ts>(vs)...)
                           : min(val2, std::forward<Ts>(vs)...);
    }

    template <typename T> T max(T val1, T val2) {
        return val1 > val2 ? val1 : val2;
    }

    template <typename T, typename... Ts> T max(T val1, T val2, Ts &&...vs) {
        return val1 > val2 ? max(val1, std::forward<Ts>(vs)...)
                           : max(val2, std::forward<Ts>(vs)...);
    }

    MATPLOT_EXPORTS
    double mean(const std::vector<double> &x);
    MATPLOT_EXPORTS
    double stddev(const std::vector<double> &x);

    MATPLOT_EXPORTS
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
        std::array<T, N> r{};
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
        std::array<T, N> r{};
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
            vector_1d r(v.size());
            std::transform(v.begin(), v.end(), r.begin(), [](const auto &x) {
                return static_cast<double>(x);
            });
            return r;
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
    detail::forward_or_copy<T, std::vector<vector_2d>>
    to_vector_3d(const T &v) {
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

    MATPLOT_EXPORTS
    double randn(double mean, double stddev);
    MATPLOT_EXPORTS
    std::vector<double> randn(size_t n, double mean, double stddev);

    /// Exponential distribution
    MATPLOT_EXPORTS
    double rande(double lambda, double multiplier = 1.0);
    MATPLOT_EXPORTS
    std::vector<double> rande(size_t n, double lambda, double multiplier = 1.0);

    /// Pareto distribution
    MATPLOT_EXPORTS
    double randp(double scale, double shape = 1.0);
    MATPLOT_EXPORTS
    std::vector<double> randp(size_t n, double scale, double shape = 1.0);

    MATPLOT_EXPORTS
    int randi(int imin, int imax);
    MATPLOT_EXPORTS
    std::vector<int> randi(size_t n, int imin, int imax);

    MATPLOT_EXPORTS
    double rand(double lower_bound, double upper_bound);
    MATPLOT_EXPORTS
    vector_1d rand(size_t n, double lower_bound, double upper_bound);
    MATPLOT_EXPORTS
    vector_2d rand(size_t rows, size_t cols, double lower_bound,
                   double upper_bound);

    MATPLOT_EXPORTS
    std::vector<double> reshape(const std::vector<std::vector<double>> &X);

    MATPLOT_EXPORTS
    std::vector<double> concat(const std::vector<double> &a,
                               const std::vector<double> &b);

    template <class... Args>
    std::vector<double> concat(const std::vector<double> &a,
                               const std::vector<double> &b, Args &&...args) {
        std::vector<double> r = concat(a, b);
        return concat(r, std::forward<Args>(args)...);
    }

    MATPLOT_EXPORTS
    double truncate(double x, double lower_bound, double upper_bound);

    template <class Arg1, class TUPLE>
    void reorder_parameter_pack_in_tuple(TUPLE &t, Arg1 x) {
        std::get<Arg1>(t) = x;
    }

    template <class Arg1, class... Args, class TUPLE>
    void reorder_parameter_pack_in_tuple(TUPLE &t, Arg1 x, Args &&...args) {
        std::get<Arg1>(t) = x;
        reorder_parameter_pack_in_tuple(t, std::forward<Args>(args)...);
    }

    MATPLOT_EXPORTS
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
            (void)ptr;
            *rank_input = ranking;
            ++rank_input;
        }
    }

    /// Cosine of argument in degreescollapse
    MATPLOT_EXPORTS
    double cosd(double degrees);

    /// Sine of argument in degreescollapse
    MATPLOT_EXPORTS
    double sind(double degrees);

    /// Sine of argument in degreescollapse
    MATPLOT_EXPORTS
    std::vector<double> deg2rad(const std::vector<double> &degrees);

    MATPLOT_EXPORTS
    double deg2rad(double rho);

    /// Round up the maximum polar value
    MATPLOT_EXPORTS
    double round_polar_max(double polar_max);

    MATPLOT_EXPORTS
    double to_positive_radian(double radian);

    /// Calculate the radians rho of a vector (u,v)
    MATPLOT_EXPORTS
    double vector_radians(double u, double v);

    /// Calculate the magnitude theta of a vector (u,v)
    MATPLOT_EXPORTS
    double vector_magnitude(double u, double v);

    /// Calculate the radians rho of a complex number u + iv
    MATPLOT_EXPORTS
    double vector_radians(std::complex<double> v);

    /// Calculate the magnitude theta of a complex number u + iv
    MATPLOT_EXPORTS
    double vector_magnitude(std::complex<double> v);

    /// Calculate cartesian coordinates of vector in polar coordinates
    MATPLOT_EXPORTS
    std::pair<double, double> pol2cart(double theta, double rho);
    MATPLOT_EXPORTS
    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, vector_1d rho);
    MATPLOT_EXPORTS
    std::pair<vector_1d, vector_1d> pol2cart(vector_1d theta, double rho);

    MATPLOT_EXPORTS
    std::pair<double, double> elliptic2cart(double theta, double rho_w,
                                            double rho_h);
    MATPLOT_EXPORTS
    std::pair<vector_1d, vector_1d> elliptic2cart(vector_1d theta, double rho_w,
                                                  double rho_h);
    MATPLOT_EXPORTS
    std::pair<vector_1d, vector_1d>
    elliptic2cart(vector_1d theta, vector_1d rho_w, vector_1d rho_h);

    MATPLOT_EXPORTS
    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x,
                                             const vector_1d &y);
    MATPLOT_EXPORTS
    std::pair<vector_2d, vector_2d> meshgrid(const vector_1d &x_and_y);

    MATPLOT_EXPORTS
    vector_1d gradient(const vector_1d &z, double spacing = 1.0);
    MATPLOT_EXPORTS
    std::pair<vector_2d, vector_2d> gradient(const vector_2d &z,
                                             double spacing = 1.0);
    MATPLOT_EXPORTS
    std::pair<vector_2d, vector_2d>
    gradient(const vector_2d &z, double spacing_x, double spacing_y);

    MATPLOT_EXPORTS
    double peaks(double x, double y);
    MATPLOT_EXPORTS
    vector_2d peaks(const vector_2d &X, const vector_2d &Y);
    MATPLOT_EXPORTS
    std::tuple<vector_2d, vector_2d, vector_2d> peaks(size_t N = 49);

    template <class T1, class T2> struct pair_hash {
        std::size_t operator()(const std::pair<T1, T2> &p) const {
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

    MATPLOT_EXPORTS
    vector_2d zeros(size_t rows, size_t cols);
    MATPLOT_EXPORTS
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
    MATPLOT_EXPORTS
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

    MATPLOT_EXPORTS
    image_channel_t rgb2gray(const image_channels_t &A);
    MATPLOT_EXPORTS
    image_channels_t gray2rgb(const image_channel_t &A,
                              const std::vector<std::vector<double>> &colormap);
    MATPLOT_EXPORTS
    image_channels_t gray2rgb(const image_channel_t &A);
    MATPLOT_EXPORTS
    image_channels_t gray2rgb(const image_channels_t &A,
                              const std::vector<std::vector<double>> &colormap);
    MATPLOT_EXPORTS
    image_channels_t gray2rgb(const image_channels_t &A);

    MATPLOT_EXPORTS
    image_channels_t
    imresize(const image_channels_t &A, double scale,
             image_interpolation m = image_interpolation::bicubic);

    MATPLOT_EXPORTS
    image_channels_t
    imresize(const image_channels_t &A, size_t height, size_t width,
             image_interpolation m = image_interpolation::bicubic);

    MATPLOT_EXPORTS
    void imwrite(const image_channels_t &A, const std::string &filename);

    MATPLOT_EXPORTS
    void imwrite(const image_channel_t &A,
                 const std::vector<std::vector<double>> &colormap,
                 const std::string &filename);

    MATPLOT_EXPORTS
    void imwrite(const image_channels_t &A, const std::string &filename);

    MATPLOT_EXPORTS
    image_channels_t imvignette(const image_channels_t &A,
                                double min_radius = 1., double exponent = 0.5);

    template <class T = double> std::vector<std::vector<T>> eye(size_t n) {
        std::vector<std::vector<T>> r(n, std::vector<T>(n, static_cast<T>(0)));
        for (size_t i = 0; i < n; ++i) {
            r[i][i] = 1.;
        }
        return r;
    }

    MATPLOT_EXPORTS
    vector_2d transpose(const vector_2d &z);

    MATPLOT_EXPORTS
    std::vector<std::string>
    tokenize(std::string_view text,
             std::string_view delimiters = " ',\n\r\t\".!?:");

    MATPLOT_EXPORTS
    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(const std::vector<std::string> &tokens,
              const std::vector<std::string> &black_list,
              size_t max_cloud_size = 100);

    MATPLOT_EXPORTS
    std::pair<std::vector<std::string>, std::vector<size_t>>
    wordcount(std::string_view text, const std::vector<std::string> &black_list,
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
    MATPLOT_EXPORTS
    ticks_results calcticks(double limits_min = -5, double limits_max = +5,
                            bool horizontal = false, double text_size = 1.25,
                            bool separateExp = true, bool log = false);

    MATPLOT_EXPORTS
    double distance(double x1, double y1, double x2, double y2);

    /// \brief Simple read-only 2d view for vector_2d.
    /// It could iterates over all vector's elements and get value using
    /// only one index (offset).
    class MATPLOT_EXPORTS vector_2d_view {
        using vector_2d = matplot::vector_2d;
        const vector_2d &_vec;
        const std::size_t _sz;

        /// \brief Helper method for determining the whole array size.
        /// Required for caching inner vector's size.
        std::size_t calculate_size(const vector_2d &vec) {
            return std::accumulate(
                vec.begin(), vec.end(), (std::size_t)(0),
                [](auto cnt, const auto &vec) { return cnt + vec.size(); });
        }

      public:
        static const double &get_element_from_offset(const vector_2d &vec,
                                                     std::size_t offset) {
            std::size_t row_size = vec[0].size();
            std::size_t cur_row = offset / row_size;
            std::size_t cur_column = offset % row_size;
            return vec[cur_row][cur_column];
        }

        static vector_2d_view from_vector_2d(const vector_2d &vec) {
            return vector_2d_view(vec);
        }
        std::size_t size() const { return _sz; }
        vector_2d_view(const vector_2d &vec)
            : _vec(vec), _sz(calculate_size(vec)) {}

        double operator()(std::size_t i, std::size_t j) {
            return _vec.at(i).at(j);
        }
        friend class iterator;

        class iterator {
            vector_2d_view *const _vec;
            std::size_t _row_offset;
            std::size_t _col_offset;

            static bool is_empty_iterator(const iterator &it) {
                return it._vec == nullptr;
            }
            static bool is_end_iterator(const iterator &it) {
                return is_empty_iterator(it) ||
                       it._row_offset * it._col_offset >= it._vec->size();
            }

          public:
            iterator(vector_2d_view *const vec)
                : _vec(vec), _row_offset(0), _col_offset(0) {}

            bool operator==(const iterator &it) const {
                return is_end_iterator(it)
                           ? is_end_iterator(*this)
                           : it._vec == _vec && it._row_offset == _row_offset &&
                                 it._col_offset == _col_offset;
            }

            auto operator*() const { return (*_vec)(_row_offset, _col_offset); }

            iterator &operator++() {
                _col_offset++;
                if (_col_offset >= _vec->_vec.at(_row_offset).size()) {
                    _col_offset = 0;
                    _row_offset++;
                }
                return *this;
            }

            // iterator traits
            using value_type = double;
        };
        iterator begin() { return iterator{this}; }
        iterator end() { return iterator{nullptr}; }
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_COMMON_H
