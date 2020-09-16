//
// Created by Alan Freitas on 10/07/20.
//

#include <matplot/axes_objects/function_line.h>
#include <matplot/util/common.h>

namespace matplot {

    function_line::function_line(class axes_type *parent) : line(parent) {}

    function_line::function_line(class axes_type *parent,
                                 const function_type &function,
                                 std::array<double, 2> x_range,
                                 std::string_view line_spec)
        : line(parent, {}, line_spec), t_range_(x_range), fn_x_(function) {}

    function_line::function_line(class axes_type *parent,
                                 const function_type &function_x,
                                 const function_type &function_y,
                                 std::array<double, 2> t_range,
                                 std::string_view line_spec)
        : line(parent, {}, line_spec), t_range_(t_range), fn_x_(function_x),
          fn_y_(function_y) {}

    function_line::function_line(class axes_type *parent,
                                 const function_type &function_x,
                                 const function_type &function_y,
                                 const function_type &function_z,
                                 std::array<double, 2> t_range,
                                 std::string_view line_spec)
        : line(parent, {}, line_spec), t_range_(t_range), fn_x_(function_x),
          fn_y_(function_y), fn_z_(function_z) {}

    std::string function_line::plot_string() {
        make_sure_data_is_preprocessed();
        return line::plot_string();
    }

    double function_line::xmax() {
        if (fn_x_ && !fn_y_ && !polar_) {
            return t_range_[1];
        } else {
            make_sure_data_is_preprocessed();
            return line::xmax();
        }
    }

    double function_line::xmin() {
        if (fn_x_ && !fn_y_ && !polar_) {
            return t_range_[0];
        } else {
            make_sure_data_is_preprocessed();
            return line::xmin();
        }
    }

    double function_line::ymin() {
        make_sure_data_is_preprocessed();
        return line::ymin();
    }

    double function_line::ymax() {
        make_sure_data_is_preprocessed();
        return line::ymax();
    }

    enum axes_object::axes_category function_line::axes_category() {
        if (polar_) {
            return axes_object::axes_category::polar;
        } else if (!fn_z_) {
            return axes_object::axes_category::two_dimensional;
        } else {
            return axes_object::axes_category::three_dimensional;
        }
    }

    const std::function<double(double)> &function_line::fn() const {
        return fn_x_;
    }

    class function_line &
    function_line::fn(const std::function<double(double)> &fn) {
        fn_x_ = fn;
        touch();
        return *this;
    }

    class function_line &function_line::tmin(double x) {
        t_range_[0] = x;
        touch();
        return *this;
    }

    class function_line &function_line::tmax(double x) {
        t_range_[1] = x;
        touch();
        return *this;
    }

    double function_line::tmin() { return t_range_[0]; }

    double function_line::tmax() { return t_range_[1]; }

    const std::array<double, 2> &function_line::t_range() const {
        return t_range_;
    }

    class function_line &
    function_line::t_range(const std::array<double, 2> &t_range) {
        t_range_ = t_range;
        touch();
        return *this;
    }

    class function_line &function_line::t_range(double t_min, double t_max) {
        return t_range({t_min, t_max});
    }

    void function_line::make_sure_data_is_preprocessed() {
        if (fn_x_ && !fn_y_) {
            bool mesh_is_ok =
                automatic_mesh_density_ ||
                (!automatic_mesh_density_ && x_data().size() == mesh_density_);
            // put the t_data directly in x_data as they are equal anyway
            if (!mesh_is_ok || x_data_.empty() ||
                x_data_.front() != t_range_[0] ||
                x_data_.back() != t_range_[1]) {
                x_data_ =
                    linspace(t_range_[0], t_range_[1],
                             automatic_mesh_density_ ? 500 : mesh_density_);
                y_data_ = transform(x_data_, fn_x_);
            }
        } else if ((fn_x_ && !fn_z_)) {
            // proprocess t_data and use it to calculate x and y data
            bool mesh_is_ok =
                automatic_mesh_density_ ||
                (!automatic_mesh_density_ && t_data_.size() == mesh_density_);
            if (!mesh_is_ok || t_data_.empty() ||
                t_data_.front() != t_range_[0] ||
                t_data_.back() != t_range_[1]) {
                t_data_ =
                    linspace(t_range_[0], t_range_[1],
                             automatic_mesh_density_ ? 500 : mesh_density_);
                x_data_ = transform(t_data_, fn_x_);
                y_data_ = transform(t_data_, fn_y_);
            }
        } else {
            bool mesh_is_ok =
                automatic_mesh_density_ ||
                (!automatic_mesh_density_ && t_data_.size() == mesh_density_);
            if (!mesh_is_ok || t_data_.empty() ||
                t_data_.front() != t_range_[0] ||
                t_data_.back() != t_range_[1]) {
                t_data_ =
                    linspace(t_range_[0], t_range_[1],
                             automatic_mesh_density_ ? 500 : mesh_density_);
                x_data_ = transform(t_data_, fn_x_);
                y_data_ = transform(t_data_, fn_y_);
                z_data_ = transform(t_data_, fn_z_);
            }
        }
    }

    size_t function_line::mesh_density() const { return mesh_density_; }

    class function_line &function_line::mesh_density(size_t mesh_density) {
        if (mesh_density != mesh_density_ || automatic_mesh_density_) {
            mesh_density_ = mesh_density;
            automatic_mesh_density_ = false;
            touch();
        }
        return *this;
    }

    bool function_line::automatic_mesh_density() const {
        return automatic_mesh_density_;
    }

    class function_line &
    function_line::automatic_mesh_density(bool automatic_mesh_density) {
        if (automatic_mesh_density_ == automatic_mesh_density) {
            automatic_mesh_density_ = automatic_mesh_density;
            touch();
        }
        return *this;
    }

} // namespace matplot