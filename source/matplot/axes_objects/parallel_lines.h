//
// Created by Alan Freitas on 15/07/20.
//

#ifndef MATPLOTPLUSPLUS_PARALLEL_LINES_H
#define MATPLOTPLUSPLUS_PARALLEL_LINES_H

#include <matplot/core/axes_object.h>
#include <matplot/core/axis.h>
#include <matplot/core/figure.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

namespace matplot {
class axes;
class parallel_lines : public axes_object {
   public:
    explicit parallel_lines(class axes* parent);
    parallel_lines(class axes* parent, const std::vector<std::vector<double>>& data, const std::string& line_spec = "");

    /// If we receive an axes_handle, we can convert it to a raw
    /// pointer because there is no ownership involved here
    template <class... Args>
    parallel_lines(const axes_handle& parent, Args... args) : parallel_lines(parent.get(), args...) {}

   public /* mandatory virtual functions */:
    std::string set_variables_string() override;
    std::string plot_string() override;
    std::string legend_string(const std::string& title) override;
    std::string data_string() override;
    std::string unset_variables_string() override;
    enum axes_object::axes_category axes_category() override;

   public /* getters and setters */:
    const line_spec& line_spec() const;
    class parallel_lines& line_spec(const class line_spec& line_spec);

    const std::vector<std::vector<double>>& data() const;
    class parallel_lines& data(const std::vector<std::vector<double>>& data);

    const std::vector<struct axis>& axis() const;
    std::vector<struct axis>& axis();
    class parallel_lines& axis(const std::vector<struct axis>& axis);

    bool visible() const;
    class parallel_lines& visible(bool visible);

    bool jitter() const;
    class parallel_lines& jitter(bool jitter);

    const std::vector<float>& line_widths() const;
    class parallel_lines& line_widths(const std::vector<float>& line_widths);

    const std::vector<double>& line_colors() const;
    class parallel_lines& line_colors(const std::vector<double>& line_colors);

   public /* getters and setters bypassing the line_spec */:
   protected:
    void maybe_update_line_spec();

   protected:
    class line_spec line_spec_;
    std::vector<std::vector<double>> data_{};
    std::vector<class axis> axis_{};
    std::vector<float> line_widths_{};
    std::vector<double> line_colors_{};
    bool jitter_{true};
    bool visible_{true};
};
}  // namespace matplot

#endif  // MATPLOTPLUSPLUS_PARALLEL_LINES_H
