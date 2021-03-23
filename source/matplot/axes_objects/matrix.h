//
// Created by Alan Freitas on 16/07/20.
//

#ifndef MATPLOTPLUSPLUS_MATRIX_H
#define MATPLOTPLUSPLUS_MATRIX_H

#include <matplot/core/figure_type.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/axis_type.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/common.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

namespace matplot {
    class axes_type;

    class matrix : public axes_object {
      public:
        enum class color_normalization { none, rows, columns };

      public:
        explicit matrix(class axes_type *parent);

        /// Heatmap
        /// Matrix of values that can be any double
        matrix(class axes_type *parent,
               const std::vector<std::vector<double>> &matrix);

        /// Matrix with an rgb image
        matrix(class axes_type *parent,
               const std::vector<std::vector<double>> &red_channel,
               const std::vector<std::vector<double>> &green_channel,
               const std::vector<std::vector<double>> &blue_channel,
               const std::vector<std::vector<double>> &alpha_channel = {});

        /// Matrix with a b&w image
        matrix(class axes_type *parent, const image_channel_t &gray_image);

        /// Matrix with an rgb image
        matrix(class axes_type *parent, const image_channel_t &red_channel,
               const image_channel_t &green_channel,
               const image_channel_t &blue_channel,
               const image_channel_t &alpha_channel = {});

        /// Matrices with an image
        matrix(class axes_type *parent, const image_channels_t &rgb_image);

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        matrix(const axes_handle &parent, Args &&... args)
            : matrix(parent.get(), std::forward<Args>(args)...) {}

        virtual ~matrix() = default;

      public /* mandatory virtual functions */:
        // std::string set_variables_string() override;
        std::string plot_string() override;
        // std::string legend_string(const std::string& title) override;
        std::string data_string() override;
        // std::string unset_variables_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        color_normalization normalization() const;
        class matrix &normalization(color_normalization normalization);

        const std::vector<std::vector<double>> &matrix_r() const;
        class matrix &
        matrix_r(const std::vector<std::vector<double>> &matrix_r);

        const std::vector<std::vector<double>> &matrix_g() const;
        class matrix &
        matrix_g(const std::vector<std::vector<double>> &matrix_g);

        const std::vector<std::vector<double>> &matrix_b() const;
        class matrix &
        matrix_b(const std::vector<std::vector<double>> &matrix_b);

        const std::vector<std::vector<double>> &matrix_a() const;
        class matrix &
        matrix_a(const std::vector<std::vector<double>> &matrix_a);

        bool always_hide_labels() const;
        class matrix &always_hide_labels(bool always_hide_labels);

        double x() const;
        class matrix &x(double x);

        double y() const;
        class matrix &y(double y);

        double w() const;
        class matrix &w(double w);

        double h() const;
        class matrix &h(double h);

        double alpha() const;
        class matrix &alpha(double alpha);

      public /* functions for matrixes */:
        /// Matrix has three channels
        bool is_rgb() const;

        /// Matrix has 4 channels
        bool is_rgba() const;

        /// A matrix might have alpha because of an
        /// alpha channels or because alpha_ != 0.
        /// In both cases we have a rgba matrix
        bool has_alpha() const;

      private:
        bool should_plot_labels();
        void setup_axes();
        std::string matrix_data_string();
        std::string image_data_string();
        std::string labels_data_string();

        inline double x_width() {
            return (w_ - 1) / static_cast<double>(matrices_[0][0].size() - 1);
        }

        inline double y_width() { return (h_ - 1) / static_cast<double>(matrices_[0].size() - 1); }

      protected:
        // Main matrix
        std::vector<std::vector<std::vector<double>>> matrices_{};

        // For heatmaps or 1 matrix
        color_normalization normalization_{color_normalization::none};

        // Matrix placement
        double x_{1};
        double y_{1};
        double w_{0.};
        double h_{0.};

        // Style
        bool always_hide_labels_{false};
        labels_handle labels_;
        double alpha_ = 0.0;

        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_MATRIX_H
