//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_AXIS_H
#define MATPLOTPLUSPLUS_AXIS_H

#include <matplot/util/handle_types.h>
#include <matplot/util/keywords.h>

#include <array>
#include <string>
#include <vector>

namespace matplot {
class axes;

/// A two dimensional axis
class axis {
 public:
  enum class axis_scale { linear, log };

 public:
  friend axes;
  axis();
  explicit axis(class axes* parent);
  explicit axis(class axes* parent, bool visible);
  axis(class axes* parent, double min, double max);
  axis(class axes* parent, double min, double max, bool visible);

 public:
  void touch();

 public:
  const std::array<double, 2>& limits() const;

  class axis& limits(const std::array<double, 2>& limits);

  bool limits_mode_auto() const;
  bool limits_mode_manual() const;
  class axis& limits_mode_auto(bool limits_mode_auto);
  class axis& limits_mode_manual(bool limits_mode_manual);
  std::string range_string() const;

  bool reverse() const;
  class axis& reverse(bool reverse);

  const color_array& color() const;
  class axis& color(const color_array& color);
  class axis& color(const std::string& color);
  class axis& color(const enum color& color);

  const std::string& label() const;
  class axis& label(const std::string& label);

  const std::string& tick_label_format() const;
  class axis& tick_label_format(const std::string& tick_label_format);

  bool tick_values_automatic() const;
  class axis& tick_values_automatic(bool tick_values_automatic);

  bool tick_values_manual() const;
  class axis& tick_values_manual(bool tick_values_manual);

  /// Get tick values
  /// This might be tick values provided by the user
  /// or tick values calculated automatically for the range
  const std::vector<double>& tick_values() const;
  class axis& tick_values(const std::vector<double>& tick_values);

  /// Get tick values
  /// This might be tick labels provided by the user
  /// or tick labels according to a format
  const std::vector<std::string>& ticklabels() const;
  class axis& ticklabels(const std::vector<std::string>& ticklabels);

  bool ticklabels_mode() const;
  class axis& ticklabels_mode(bool ticklabels_mode);

  float tickangle() const;
  class axis& tickangle(float tickangle);

  axis_scale scale() const;
  class axis& scale(axis_scale scale);

  float tick_length() const;
  class axis& tick_length(float tick_length);

  bool zero_axis() const;
  class axis& zero_axis(bool zero_axis);

  bool geographic() const;
  class axis& geographic(bool geographic);

  bool on_axis() const;
  class axis& on_axis(bool on_axis);

  const std::string& label_weight() const;
  class axis& label_weight(const std::string& label_weight);

  float label_font_size() const;
  class axis& label_font_size(float label_font_size);

  const color_array& label_color() const;
  class axis& label_color(const color_array& label_color);
  template <class T>
  class axis& label_color(T c) {
    label_color(to_array(c));
    return *this;
  }

  bool visible() const;
  class axis& visible(bool visible);

 public:
  std::string label_string() const;
  std::string tick_values_string(bool minor_ticks = false) const;
  std::string tick_rotate_string() const;

 private:
  // range
  std::array<double, 2> limits_{0, 2};
  bool limits_mode_auto_{true};
  bool reverse_{false};
  axis_scale scale_{axis_scale::linear};
  float exponent{0};
  bool geographic_{false};

  // tics
  std::vector<double> tick_values_{0,      0.2000, 0.4000, 0.6000, 0.8000, 1,
                                   1.2000, 1.4000, 1.6000, 1.8000, 2};
  std::string tick_label_format_{"%g"};
  bool tick_values_automatic_{true};
  float tickangle_{0};
  std::vector<std::string> ticklabels_;
  bool ticklabels_mode_{true};
  std::array<float, 4> color_{0, 0.15, 0.15, 0.15};
  float tick_length_{0.75};

  // minor tics
  bool minor_tick_{false};
  std::vector<double> minor_tick_values_{
      0.0500, 0.1000, 0.1500, 0.2500, 0.3000, 0.3500, 0.4500, 0.5000,
      0.5500, 0.6500, 0.7000, 0.7500, 0.8500, 0.9000, 0.9500, 1.0500,
      1.1000, 1.1500, 1.2500, 1.3000, 1.3500, 1.4500, 1.5000, 1.5500,
      1.6500, 1.7000, 1.7500, 1.8500, 1.9000, 1.9500};

  // font
  std::string font_{"Helvetica"};
  float font_size_{10};
  std::string font_weight_{"normal"};

  // label
  std::string label_{""};
  std::string label_weight_{"normal"};
  float label_font_size_{11};
  color_array label_color_{0, 0, 0, 0};

  // line
  float line_width_{0.5};

  // draw zero axis
  bool zero_axis_{false};

  // Draw the tics on the axis
  bool on_axis_{false};

  bool visible_{true};

  // parent xlim
  class axes* parent_;
};

}  // namespace matplot

#endif  // MATPLOTPLUSPLUS_AXIS_H
