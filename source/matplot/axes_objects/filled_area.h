//
// Created by Alan Freitas on 09/07/20.
//

#ifndef MATPLOTPLUSPLUS_FILLED_AREA_H
#define MATPLOTPLUSPLUS_FILLED_AREA_H

#include <matplot/detail/config.h>
#include <matplot/axes_objects/line.h>

namespace matplot {
    class axes_type;

    class MATPLOT_EXPORTS filled_area : public line {
      public:
        enum class type { vertical, horizontal, both };

      public:
        explicit filled_area(class axes_type *parent);

        /// Construct with x and y error
        filled_area(class axes_type *parent, const std::vector<double> &x,
                    const std::vector<double> &y,
                    const std::vector<double> &base_values = {0.},
                    bool stacked = true, std::string_view line_spec = "k-");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership of the xlim
        template <class... Args>
        filled_area(const axes_handle &parent, Args&&... args)
            : filled_area(parent.get(), std::forward<Args>(args)...) {}

      public /* override the plotting function for filled_area */:
        std::string plot_string() override;
        std::string data_string() override;
        enum axes_object::axes_category axes_category() override;

      public /* methods for filled_area only */:
        bool stacked() const;
        class filled_area &stacked(bool stacked);

        const std::vector<double> &base_data() const;
        class filled_area &base_data(const std::vector<double> &base_data);

        bool plot_base_line() const;
        class filled_area &plot_base_line(bool plot_base_line);

        const color_array &face_color() const;
        class filled_area &face_color(const color_array &face_color);

        bool fill_user_color() const;
        class filled_area &fill_user_color(bool fill_user_color);

      private:
        void maybe_update_face_color();

      protected:
        bool stacked_{true};

        std::vector<double> base_data_{0.};
        bool plot_base_line_{true};

        color_array face_color_{0, 0, 0, 0};
        bool fill_user_color_{false};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_FILLED_AREA_H
