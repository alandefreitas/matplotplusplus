//
// Created by Alan Freitas on 09/07/20.
//

#ifndef MATPLOTPLUSPLUS_STAIR_H
#define MATPLOTPLUSPLUS_STAIR_H

#include <matplot/detail/config.h>
#include <matplot/axes_objects/line.h>

namespace matplot {
    class axes_type;

    class MATPLOT_EXPORTS stair : public line {
      public:
        enum class stair_style {
            // trace change in x, then change in y
            trace_x_first,
            // trace change in y, then change in x
            trace_y_first,
            // histogram mode: trace
            //     (i) half change in x,
            //     (ii) then change in y,
            //     (iii) then half the change in x
            histogram,
            // fill change in x, then fill change in y
            fill
        };

      public:
        explicit stair(class axes_type *parent);
        stair(class axes_type *parent, const std::vector<double> &y_data,
              std::string_view line_spec = "");
        stair(class axes_type *parent, const std::vector<double> &x_data,
              const std::vector<double> &y_data,
              std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        stair(const axes_handle &parent, Args&&... args)
            : stair(parent.get(), std::forward<Args>(args)...) {}

      public /* override the plotting function for stair */:
        std::string plot_string() override;
        //        std::string data_string() override;
        enum axes_object::axes_category axes_category() override;

      public /* methods for stair only */:
        stair_style stair_style() const;
        class stair &stair_style(enum stair_style stair_style);

        bool fill() const;
        class stair &fill(bool fill);

      protected:
        std::vector<line_spec::style_to_plot> styles_to_plot() override;

      protected:
        enum stair_style stair_style_ { stair_style::trace_x_first };
        bool fill_{false};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_STAIR_H
