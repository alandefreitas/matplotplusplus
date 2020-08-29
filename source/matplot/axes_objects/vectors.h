//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_VECTORS_H
#define MATPLOTPLUSPLUS_VECTORS_H

#include <matplot/core/axes_object.h>
#include <matplot/core/figure.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

#include <array>

namespace matplot {
class axes;
class vectors : public axes_object {
 public:
  explicit vectors(class axes* parent);

  /// Origin xy = (0,0), u = {1,...n}, v = {v_data}
  vectors(class axes* parent, const std::vector<double>& v_data,
          const std::string& line_spec = "");

  /// Origin xy = (0,0), u = {u_data}, v = {v_data}
  vectors(class axes* parent, const std::vector<double>& u_data,
          const std::vector<double>& v_data, const std::string& line_spec = "");

  /// Origin xy = (0,0,0), u = {u_data}, v = {v_data}, w = {w_data}
  vectors(class axes* parent, const std::vector<double>& u_data,
          const std::vector<double>& v_data, const std::vector<double>& w_data,
          const std::string& line_spec = "");

  vectors(class axes* parent, const std::vector<double>& x_data,
          const std::vector<double>& y_data, const std::vector<double>& u_data,
          const std::vector<double>& v_data, const std::string& line_spec = "");

  vectors(class axes* parent, const std::vector<double>& x_data,
          const std::vector<double>& y_data, const std::vector<double>& z_data,
          const std::vector<double>& u_data, const std::vector<double>& v_data,
          const std::vector<double>& w_data, const std::string& line_spec = "");

  /// If we receive an axes_handle, we can convert it to a raw
  /// pointer because there is no ownership involved here
  template <class... Args>
  vectors(const axes_handle& parent, Args... args)
      : vectors(parent.get(), args...) {}

 public /* mandatory virtual functions */:
  std::string plot_string() override;
  std::string legend_string(const std::string& title) override;
  std::string data_string() override;
  double xmax() override;
  double xmin() override;
  double ymax() override;
  double ymin() override;
  enum axes_object::axes_category axes_category() override;

 public /* getters and setters */:
  class vectors& line_style(const std::string& line_spec);

  const line_spec& line_spec() const;
  class line_spec& line_spec();
  class vectors& line_spec(const class line_spec& line_spec);

  const std::vector<double>& y_data() const;
  class vectors& y_data(const std::vector<double>& y_data);

  const std::vector<double>& x_data() const;
  class vectors& x_data(const std::vector<double>& x_data);

  const std::vector<double>& z_data() const;
  class vectors& z_data(const std::vector<double>& z_data);

  const std::vector<size_t>& marker_indices() const;
  class vectors& marker_indices(const std::vector<size_t>& marker_indices);

  bool use_y2() const;
  class vectors& use_y2(bool use_y_2);

  bool impulse() const;
  class vectors& impulse(bool impulse);

  bool fill() const;
  class vectors& fill(bool fill);

  bool use_y_2() const;
  class vectors& use_y_2(bool use_y_2);

  bool polar() const;
  class vectors& polar(bool polar);

  bool visible() const;
  class vectors& visible(bool visible);

 public /* getters and setters bypassing the line_spec */:
  float line_width() const;
  class vectors& line_width(float line_width);

  enum line_spec::marker_style marker_style() const;
  template <class T>
  class vectors& marker_style(T marker_style) {
    line_spec_.marker_style(marker_style);
    return *this;
  }

  enum line_spec::marker_style marker() const;
  template <class T>
  class vectors& marker(T marker) {
    line_spec_.marker(marker);
    return *this;
  }

  float marker_size() const;
  class vectors& marker_size(float size);
  class vectors& marker_size(const std::vector<float>& size_vector);
  class vectors& marker_size(const std::vector<double>& size_vector);

  bool marker_face() const;
  class vectors& marker_face(bool size);

  const std::array<float, 4>& color() const;
  template <class T>
  class vectors& color(T c) {
    line_spec().color(c);
    return *this;
  }
  inline class vectors& color(std::initializer_list<float> c) {
    line_spec().color(c);
    return *this;
  }

  const std::array<float, 4>& marker_color() const;

  template <class T>
  class vectors& marker_color(T c) {
    line_spec().marker_color(c);
    return *this;
  }

  inline class vectors& marker_color(std::initializer_list<float> c) {
    line_spec().marker_color(c);
    return *this;
  }

  inline class vectors& marker_colors(const std::vector<double>& cs) {
    marker_colors_ = cs;
    touch();
    return *this;
  }

  const std::array<float, 4>& marker_face_color() const;

  template <class T>
  class vectors& marker_face_color(T c) {
    line_spec().marker_face_color(c);
    return *this;
  }

  inline class vectors& marker_face_color(std::initializer_list<float> c) {
    line_spec().marker_face_color(c);
    return *this;
  }

  inline float marker_face_alpha() { return line_spec().marker_face_alpha(); }

  inline class vectors& marker_face_alpha(float a) {
    line_spec().marker_face_alpha(a);
    return *this;
  }

 protected:
  void maybe_update_line_spec();

 protected:
  /// Line style
  class line_spec line_spec_;

  /// Vector origins
  std::vector<double> y_data_{};
  std::vector<double> x_data_{};
  std::vector<double> z_data_{};

  /// Vector components
  std::vector<double> u_data_{};
  std::vector<double> v_data_{};
  std::vector<double> w_data_{};

  /// Positions at which we want markers to appear
  std::vector<size_t> marker_indices_{};
  std::vector<float> marker_sizes_{};
  std::vector<double> marker_colors_{};

  /// Draw line as impulse
  bool impulse_{false};

  /// Draw line as filled area
  bool fill_{false};

  /// Use the y2 xlim
  bool use_y2_{false};

  /// This line is meant for a polar plot
  bool polar_{false};

  /// True if visible
  bool visible_{true};
};
}  // namespace matplot

#endif  // MATPLOTPLUSPLUS_VECTORS_H
