//
// Created by Alan Freitas on 17/07/20.
//

#include <cmath>
#include <matplot/axes_objects/matrix.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {
    matrix::matrix(class axes_type *parent) : axes_object(parent) {}

    matrix::matrix(class axes_type *parent,
                   const std::vector<std::vector<double>> &matrix)
        : axes_object(parent), matrices_({matrix}) {
        // Matrix does not seem to be an image.
        // If this will be the first object in the xlim, prepare xlim
        // for a heatmap.
        always_hide_labels_ = false;
        x_ = y_ = 1;
        parent_->y_axis().reverse(true);
        auto [hi, wi] = size(matrices_[0]);
        h_ = static_cast<double>(hi);
        w_ = static_cast<double>(wi);
    }

    matrix::matrix(class axes_type *parent,
                   const std::vector<std::vector<double>> &red_channel,
                   const std::vector<std::vector<double>> &green_channel,
                   const std::vector<std::vector<double>> &blue_channel,
                   const std::vector<std::vector<double>> &alpha_channel)
        : axes_object(parent),
          matrices_(alpha_channel.empty()
                        ? std::vector{red_channel, green_channel, blue_channel}
                        : std::vector{red_channel, green_channel, blue_channel,
                                      alpha_channel}) {
        // Matrix seems to be an image
        // Leave the xlim as it is
        parent_->y_axis().reverse(true);
        always_hide_labels_ = true;
        x_ = y_ = 1;
        auto [hi, wi] = size(matrices_[0]);
        h_ = static_cast<double>(hi);
        w_ = static_cast<double>(wi);
    }

    matrix::matrix(class axes_type *parent, const image_channel_t &gray_image)
        : axes_object(parent), matrices_({to_vector_2d(gray_image)}) {
        // This seems to be an image because the matrix is unsigned char
        parent_->y_axis().reverse(true);
        always_hide_labels_ = true;
        x_ = y_ = 1;
        auto [hi, wi] = size(matrices_[0]);
        h_ = static_cast<double>(hi);
        w_ = static_cast<double>(wi);
    }

    matrix::matrix(class axes_type *parent, const image_channel_t &red_channel,
                   const image_channel_t &green_channel,
                   const image_channel_t &blue_channel,
                   const image_channel_t &alpha_channel)
        : axes_object(parent),
          matrices_(alpha_channel.empty()
                        ? std::vector{to_vector_2d(red_channel),
                                      to_vector_2d(green_channel),
                                      to_vector_2d(blue_channel)}
                        : std::vector{to_vector_2d(red_channel),
                                      to_vector_2d(green_channel),
                                      to_vector_2d(blue_channel),
                                      to_vector_2d(alpha_channel)}) {
        // This seems to be an image because the matrices are unsigned char
        parent_->y_axis().reverse(true);
        always_hide_labels_ = true;
        x_ = y_ = 1;
        auto [hi, wi] = size(matrices_[0]);
        h_ = static_cast<double>(hi);
        w_ = static_cast<double>(wi);
    }

    /// Constructor for all channels at once
    matrix::matrix(class axes_type *parent, const image_channels_t &image)
        : axes_object(parent), matrices_(to_vector_3d(image)) {
        parent_->y_axis().reverse(true);
        always_hide_labels_ = true;
        x_ = y_ = 1;
        auto [hi, wi] = size(matrices_[0]);
        h_ = static_cast<double>(hi);
        w_ = static_cast<double>(wi);
    }

    std::string matrix::plot_string() {
        std::string res = "'-' with";
        if (!has_alpha()) {
            if (matrices_.size() < 3) {
                // image with colors from colormap
                res += " image";
            } else {
                // 3 components for each point
                res += " rgbimage";
            }
        } else {
            // 4 components for each point
            // even if matrices_.size() == 1
            res += " rgbalpha";
        }

        if (should_plot_labels()) {
            // low values in black
            res += ", '-' with labels font \"" + escape(parent_->font()) + "," +
                   num2str(round(parent_->font_size())) +
                   "\" textcolor 'black'";
            // high values in white
            res += ", '-' with labels font \"" + escape(parent_->font()) + "," +
                   num2str(round(parent_->font_size())) +
                   "\" textcolor 'white'";
        }
        return res;
    }

    bool matrix::should_plot_labels() {
        if (always_hide_labels_ || matrices_.size() > 1) {
            return false;
        } else {
            return matrices_[0].size() < 20 && matrices_[0][0].size() < 30;
        }
    }

    std::string matrix::matrix_data_string() {
        // single matrix
        auto &matrix_ = matrices_[0];

        // calculate min/max row/cols if normalizing
        std::vector<double> value_max;
        std::vector<double> value_min;
        if (normalization_ == color_normalization::columns) {
            value_max.resize(matrix_[0].size());
            value_min.resize(matrix_[0].size());
            for (size_t i = 0; i < matrix_[0].size(); ++i) {
                value_max[i] = matrix_[0][i];
                value_min[i] = matrix_[0][i];
                for (size_t j = 0; j < matrix_.size(); ++j) {
                    if (matrix_[j][i] > value_max[i]) {
                        value_max[i] = matrix_[j][i];
                    }
                    if (matrix_[j][i] < value_min[i]) {
                        value_min[i] = matrix_[j][i];
                    }
                }
            }
        } else if (normalization_ == color_normalization::rows) {
            value_max.resize(matrix_.size());
            value_min.resize(matrix_.size());
            for (size_t i = 0; i < matrix_.size(); ++i) {
                value_max[i] = matrix_[i][0];
                value_min[i] = matrix_[i][0];
                for (size_t j = 0; j < matrix_[0].size(); ++j) {
                    if (matrix_[i][j] > value_max[i]) {
                        value_max[i] = matrix_[i][j];
                    }
                    if (matrix_[i][j] < value_min[i]) {
                        value_min[i] = matrix_[i][j];
                    }
                }
            }
        }

        // stream matrix
        std::stringstream ss;
        double x_width_ = x_width();
        double y_width_ = y_width();
        const auto &[cb_min, cb_max] = parent_->color_box_range();
        bool use_cb_range = cb_min != cb_max;
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < matrix_[i].size(); ++j) {
                // z will be normalized
                double z = matrix_[i][j];
                switch (normalization_) {
                case color_normalization::none:
                    break;
                case color_normalization::columns:
                    z -= value_min[j];
                    z /= value_max[j] - value_min[j];
                    break;
                case color_normalization::rows:
                    z -= value_min[i];
                    z /= value_max[i] - value_min[i];
                    break;
                }
                ss << "    " << x_ + x_width_ * j << "  " << y_ + y_width_ * i;
                if (alpha_ == 0.) {
                    ss << "  " << z;
                } else {
                    color_array c = parent_->colormap_interpolation(
                        z, use_cb_range ? cb_min : 0.,
                        use_cb_range ? cb_max : 255);
                    ss << "  " << c[1] * 255 << "  " << c[2] * 255 << "  "
                       << c[3] * 255 << "  " << (1. - alpha_) * 255;
                }
                ss << "\n";
            }
            ss << "\n";
        }
        ss << "    e\n";

        if (should_plot_labels()) {
            // find matrix max and min
            double minm = matrix_[0][0];
            double maxm = matrix_[0][0];
            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    if (matrix_[i][j] > maxm) {
                        maxm = matrix_[i][j];
                    }
                    if (matrix_[i][j] < minm) {
                        minm = matrix_[i][j];
                    }
                }
            }
            double threshold = minm + 0.7 * (maxm - minm);

            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    double normalized_value = matrix_[i][j];
                    double normalized_threshold = threshold;
                    if (normalization_ == color_normalization::columns) {
                        normalized_value = (normalized_value - value_min[j]) /
                                           (value_max[j] - value_min[j]);
                        normalized_threshold = 0.7;
                    } else if (normalization_ == color_normalization::rows) {
                        normalized_value = (normalized_value - value_min[i]) /
                                           (value_max[i] - value_min[i]);
                        normalized_threshold = 0.7;
                    }

                    if (normalized_value <= normalized_threshold) {
                        ss << "    " << x_ + x_width_ * j << "  "
                           << y_ + y_width_ * i << "  \"" << matrix_[i][j]
                           << "\"\n";
                    }
                }
                ss << "\n";
            }
            ss << "    e\n";

            for (size_t i = 0; i < matrix_.size(); ++i) {
                for (size_t j = 0; j < matrix_[i].size(); ++j) {
                    double normalized_value = matrix_[i][j];
                    double normalized_threshold = threshold;
                    if (normalization_ == color_normalization::columns) {
                        normalized_value = (normalized_value - value_min[j]) /
                                           (value_max[j] - value_min[j]);
                        normalized_threshold = 0.7;
                    } else if (normalization_ == color_normalization::rows) {
                        normalized_value = (normalized_value - value_min[i]) /
                                           (value_max[i] - value_min[i]);
                        normalized_threshold = 0.7;
                    }

                    if (normalized_value > normalized_threshold) {
                        ss << "    " << x_ + x_width_ * j << "  "
                           << y_ + y_width_ * i << "  \"" << matrix_[i][j]
                           << "\"\n";
                    }
                }
                ss << "\n";
            }
            ss << "    e\n";
        }

        return ss.str();
    }

    std::string matrix::image_data_string() {
        std::stringstream ss;
        auto [h, w] = size(matrices_[0]);
        double x_width_ = x_width();
        double y_width_ = y_width();
        for (size_t i = 0; i < w; ++i) {
            for (size_t j = 0; j < h; ++j) {
                ss << "    " << x_ + x_width_ * i;
                ss << "  " << y_ + y_width_ * j;
                ss << "  " << static_cast<int>(matrices_[0][j][i]);
                if (matrices_.size() >= 3) {
                    ss << "  " << static_cast<int>(matrices_[1][j][i]);
                    ss << "  " << static_cast<int>(matrices_[2][j][i]);
                }
                if (has_alpha()) {
                    ss << "  "
                       << static_cast<int>(
                              (1 - alpha_) *
                              (is_rgba() ? matrices_[3][j][i] : 255.));
                }
                ss << "\n";
            }
        }
        ss << "    e\n";
        return ss.str();
    }

    std::string matrix::data_string() {
        return matrices_.size() > 1 ? image_data_string()
                                    : matrix_data_string();
    }

    double matrix::xmax() { return (x_ + w_ - 1) + x_width() / 2; }

    double matrix::xmin() { return x_ - x_width() / 2; }

    double matrix::ymax() { return (y_ + h_ - 1) + y_width() / 2; }

    double matrix::ymin() { return y_ - y_width() / 2; }

    enum axes_object::axes_category matrix::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    matplot::matrix::color_normalization matrix::normalization() const {
        return normalization_;
    }

    class matrix &
    matrix::normalization(matplot::matrix::color_normalization normalization) {
        normalization_ = normalization;
        touch();
        return *this;
    }

    bool matrix::is_rgb() const { return matrices_.size() == 3; }

    bool matrix::is_rgba() const { return matrices_.size() == 4; }

    bool matrix::has_alpha() const {
        return matrices_.size() == 4 || alpha_ != 0.;
    }

    const std::vector<std::vector<double>> &matrix::matrix_r() const {
        return matrices_[0];
    }

    class matrix &
    matrix::matrix_r(const std::vector<std::vector<double>> &matrix_r) {
        matrices_[0] = matrix_r;
        touch();
        return *this;
    }

    const std::vector<std::vector<double>> &matrix::matrix_g() const {
        return matrices_[1];
    }

    class matrix &
    matrix::matrix_g(const std::vector<std::vector<double>> &matrix_g) {
        if (matrices_.size() < 2) {
            matrices_.resize(2);
        }
        matrices_[1] = matrix_g;
        touch();
        return *this;
    }

    const std::vector<std::vector<double>> &matrix::matrix_b() const {
        return matrices_[2];
    }

    class matrix &
    matrix::matrix_b(const std::vector<std::vector<double>> &matrix_b) {
        if (matrices_.size() < 3) {
            matrices_.resize(3);
        }
        matrices_[2] = matrix_b;
        touch();
        return *this;
    }

    const std::vector<std::vector<double>> &matrix::matrix_a() const {
        return matrices_[3];
    }

    class matrix &
    matrix::matrix_a(const std::vector<std::vector<double>> &matrix_a) {
        if (matrices_.size() < 4) {
            matrices_.resize(4);
        }
        matrices_[3] = matrix_a;
        touch();
        return *this;
    }

    bool matrix::always_hide_labels() const { return always_hide_labels_; }

    class matrix &matrix::always_hide_labels(bool always_hide_labels) {
        always_hide_labels_ = always_hide_labels;
        touch();
        return *this;
    }

    double matrix::x() const { return x_; }

    class matrix &matrix::x(double x) {
        x_ = x;
        touch();
        return *this;
    }

    double matrix::y() const { return y_; }

    class matrix &matrix::y(double y) {
        y_ = y;
        touch();
        return *this;
    }

    double matrix::w() const { return w_; }

    class matrix &matrix::w(double w) {
        w_ = w;
        touch();
        return *this;
    }

    double matrix::h() const { return h_; }

    class matrix &matrix::h(double h) {
        h_ = h;
        touch();
        return *this;
    }

    double matrix::alpha() const { return alpha_; }

    class matrix &matrix::alpha(double alpha) {
        alpha_ = alpha;
        touch();
        return *this;
    }
} // namespace matplot