//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_FUNCTION_LINE_H
#define MATPLOTPLUSPLUS_FUNCTION_LINE_H

#include <matplot/axes_objects/line.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/handle_types.h>
#include <string>

namespace matplot {
    class function_line : public line {
      public:
        using function_type = std::function<double(double)>;

      public:
        explicit function_line(class axes_type *parent);

        function_line(class axes_type *parent, const function_type &equation,
                      std::array<double, 2> x_range = {-5, 5},
                      std::string_view line_spec = "");

        function_line(class axes_type *parent, const function_type &function_x,
                      const function_type &function_y,
                      std::array<double, 2> t_range = {-5, 5},
                      std::string_view line_spec = "");

        function_line(class axes_type *parent, const function_type &function_x,
                      const function_type &function_y,
                      const function_type &function_z,
                      std::array<double, 2> t_range = {-5, 5},
                      std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        function_line(const axes_handle &parent, Args&&... args)
            : function_line(parent.get(), std::forward<Args>(args)...) {}

      public:
        std::string plot_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public:
        const function_type &fn() const;
        class function_line &fn(const function_type &fn);

        class function_line &tmin(double x);
        class function_line &tmax(double x);
        double tmin();
        double tmax();

        const std::array<double, 2> &t_range() const;
        class function_line &t_range(const std::array<double, 2> &t_range);
        class function_line &t_range(double t_min, double t_max);

        size_t mesh_density() const;
        class function_line &mesh_density(size_t mesh_density);

        bool automatic_mesh_density() const;
        class function_line &
        automatic_mesh_density(bool automatic_mesh_density);

      private:
        void make_sure_data_is_preprocessed();

      private:
        std::array<double, 2> t_range_;
        std::vector<double> t_data_{};
        size_t mesh_density_{30};
        bool automatic_mesh_density_{true};
        function_type fn_x_{nullptr};
        function_type fn_y_{nullptr};
        function_type fn_z_{nullptr};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_FUNCTION_LINE_H
