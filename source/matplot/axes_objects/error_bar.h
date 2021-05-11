//
// Created by Alan Freitas on 09/07/20.
//

#ifndef MATPLOTPLUSPLUS_ERROR_BAR_H
#define MATPLOTPLUSPLUS_ERROR_BAR_H

#include <matplot/axes_objects/line.h>

namespace matplot {
    class axes_type;

    class error_bar : public line {
      public:
        enum class type { vertical, horizontal, both };

      public:
        explicit error_bar(class axes_type *parent);

        /// Construct with x and y error
        error_bar(class axes_type *parent, const std::vector<double> &x,
                  const std::vector<double> &y,
                  const std::vector<double> &y_neg_delta,
                  const std::vector<double> &y_pos_delta,
                  const std::vector<double> &x_neg_delta,
                  const std::vector<double> &x_pos_delta,
                  std::string_view line_spec = "");

        /// Construct with y error only
        error_bar(class axes_type *parent, const std::vector<double> &x_data,
                  const std::vector<double> &y_data,
                  const std::vector<double> &error,
                  error_bar::type type = error_bar::type::vertical,
                  std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership of the xlim
        template <class... Args>
        error_bar(const axes_handle &parent, Args&&... args)
            : error_bar(parent.get(), std::forward<Args>(args)...) {}

      public /* override the plotting function for error_bar */:
        std::string set_variables_string() override;
        std::string plot_string() override;
        std::string data_string() override;
        bool requires_colormap() override;
        std::string unset_variables_string() override;
        enum axes_object::axes_category axes_category() override;

      public /* methods for error_bar only */:
        const std::vector<double> &x_negative_delta() const;
        class error_bar &
        x_negative_delta(const std::vector<double> &x_negative_delta);

        const std::vector<double> &x_positive_delta() const;
        class error_bar &
        x_positive_delta(const std::vector<double> &x_positive_delta);

        const std::vector<double> &y_negative_delta() const;
        class error_bar &
        y_negative_delta(const std::vector<double> &y_negative_delta);

        const std::vector<double> &y_positive_delta() const;
        class error_bar &
        y_positive_delta(const std::vector<double> &y_positive_delta);

        float filled_curve_alpha() const;
        class error_bar &filled_curve_alpha(float filled_curve_alpha);

        bool filled_curve() const;
        class error_bar &filled_curve(bool filled_curve);

        float cap_size() const;
        class error_bar &cap_size(float cap_size);

      protected:
        std::vector<double> x_negative_delta_{};
        std::vector<double> x_positive_delta_{};
        std::vector<double> y_negative_delta_{};
        std::vector<double> y_positive_delta_{};

        bool filled_curve_{false};
        float filled_curve_alpha_{0.9f};

        float cap_size_{3.};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_ERROR_BAR_H
