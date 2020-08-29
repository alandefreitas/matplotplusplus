//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_CHARTOBJECT_H
#define MATPLOTPLUSPLUS_CHARTOBJECT_H

#include <memory>
#include <vector>
#include <string>

namespace matplot {
    class axes;
    using axes_handle = std::shared_ptr<class axes>;

    /// Abstract class for the objects we put in the xlim
    class axes_object {
      public:
        enum class axes_category {
            two_dimensional,
            three_dimensional,
            three_dimensional_map,
            polar
        };

      public:
        explicit axes_object(class axes *parent);

        explicit axes_object(axes_handle parent);

      public:
        virtual std::string set_variables_string();

        // Plot command for this object
        virtual std::string plot_string() = 0;

        // Create a legend string for this object
        // https://stackoverflow.com/questions/60617211/how-to-put-a-rectangle-in-the-key-with-same-hue-as-a-shaded-area-in-gnuplot/60624922#60624922
        virtual std::string legend_string(const std::string &legend);

        // Take a number of legends from the range [legends_begin, legends_end]
        // and advance the legends_begin iterator.
        // Objects might want to take care of more than one legend.
        // They can overload this method to do that.
        // If not, they can just inherit the simple legend_string method.
        virtual std::string
        legend_string(std::vector<std::string>::iterator &legends_begin,
                      std::vector<std::string>::iterator &legends_end);

        virtual std::string data_string();
        virtual std::string unset_variables_string();
        virtual double xmax();
        virtual double xmin();
        virtual double ymax();
        virtual double ymin();
        virtual double zmax();
        virtual double zmin();
        virtual axes_category axes_category();
        virtual bool requires_colormap();
        virtual std::string tag();
        virtual void tag(const std::string &);
        bool is_3d();
        bool is_3d_map();
        bool is_2d();
        bool is_polar();

      public:
        const class axes *parent() const;
        class axes *&parent();
        void parent(class axes *&parent);
        void touch();

        // Objects might have their own display name
        // In this case, we use the display name for legends
        // instead of the strings in the legends object
        const std::string &display_name() const;
        void display_name(const std::string &display_name);

      protected:
        std::string tag_{"axes_object"};
        std::string display_name_{""};
        class axes *parent_;
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_CHARTOBJECT_H
