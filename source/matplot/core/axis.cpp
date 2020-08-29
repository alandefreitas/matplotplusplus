//
// Created by Alan Freitas on 2020-07-05.
//

#include <matplot/core/axes.h>
#include <matplot/core/axis.h>
#include <matplot/core/figure.h>
#include <matplot/util/colors.h>
#include <matplot/util/common.h>

#include <cmath>

namespace matplot {

axis::axis() : axis(nullptr, inf, inf) {}

axis::axis(class axes* parent) : axis(parent, inf, inf) {}

axis::axis(class axes* parent, bool visible)
    : axis(parent, inf, inf, visible) {}

axis::axis(class axes* parent, double min, double max)
    : axis(parent, min, max, true) {}

axis::axis(class axes* parent, double min, double max, bool visible)
    : parent_(parent), limits_({min, max}), visible_(visible) {}

void axis::touch() { parent_->touch(); }

const std::array<double, 2>& axis::limits() const { return limits_; }

class axis& axis::limits(const std::array<double, 2>& limits) {
  limits_ = limits;
  limits_mode_manual(true);
  touch();
  return *this;
}

bool axis::limits_mode_auto() const {
  return limits_mode_auto_ || (!isfinite(limits_[0]) && !isfinite(limits_[0]));
}

bool axis::limits_mode_manual() const { return !limits_mode_auto_; }

class axis& axis::limits_mode_auto(bool limits_mode_auto) {
  limits_mode_auto_ = limits_mode_auto;
  parent_->touch();
  return *this;
}

class axis& axis::limits_mode_manual(bool limits_mode_manual) {
  limits_mode_auto_ = !limits_mode_manual;
  parent_->touch();
  return *this;
}

std::string axis::range_string() const {
  if (!reverse_) {
    std::string r = "[";
    if (!limits_mode_auto_ && isfinite(limits_[0])) {
      r += std::to_string(limits_[0]);
    }
    r += ":";
    if (!limits_mode_auto_ && isfinite(limits_[1])) {
      r += std::to_string(limits_[1]);
    }
    r += "] noreverse";
    return r;
  } else {
    std::string r = "[";
    if (!limits_mode_auto_ && isfinite(limits_[1])) {
      r += std::to_string(limits_[1]);
    }
    r += ":";
    if (!limits_mode_auto_ && isfinite(limits_[0])) {
      r += std::to_string(limits_[0]);
    }
    r += "] reverse";
    return r;
  }
}

bool axis::reverse() const { return reverse_; }

class axis& axis::reverse(bool reverse) {
  reverse_ = reverse;
  parent_->touch();
  return *this;
}

const color_array& axis::color() const { return color_; }

class axis& axis::color(const color_array& color) {
  color_ = color;
  touch();
  return *this;
}

class axis& axis::color(const std::string& c) {
  color(string_to_color(c));
  return *this;
}

class axis& axis::color(const enum color& c) {
  return color(to_array(c));
}

const std::string& axis::label() const { return label_; }

class axis& axis::label(const std::string& label) {
  label_ = label;
  touch();
  return *this;
}

std::string axis::label_string() const {
  std::string cmd = " \"";
  if (label_weight_ == "bold") {
    cmd += "{/:Bold ";
  }
  if (label_weight_ == "italic") {
    cmd += "{/:Italic ";
  }
  cmd += escape(label());
  if (label_weight_ == "bold") {
    cmd += "}";
  }
  if (label_weight_ == "italic") {
    cmd += "}";
  }
  cmd += "\"";
  if (parent_->parent()->backend()->supports_fonts()) {
    cmd += " font \"" + escape(parent_->font()) + "," +
           num2str(unsigned(label_font_size_)) + "\"";
    cmd += " textcolor \"" + to_string(label_color_) + "\"";
  }
  return cmd;
}

const std::string& axis::tick_label_format() const {
  return tick_label_format_;
}

class axis& axis::tick_label_format(const std::string& tick_label_format) {
  if (tick_label_format == "usd") {
    tick_label_format_ = "$%.2f";
  } else if (tick_label_format == "degrees") {
    tick_label_format_ = "%g°";
  } else if (tick_label_format == "percentage") {
    tick_label_format_ = "%g%%";
  } else if (tick_label_format == "auto") {
    tick_label_format_ = "%g";
  } else {
    tick_label_format_ = tick_label_format;
  }
  touch();
  return *this;
}

bool axis::tick_values_automatic() const { return tick_values_automatic_; }

class axis& axis::tick_values_automatic(bool tick_values_automatic) {
  tick_values_automatic_ = tick_values_automatic;
  touch();
  return *this;
}

bool axis::tick_values_manual() const { return !tick_values_automatic_; }

class axis& axis::tick_values_manual(bool tick_values_manual) {
  tick_values_automatic_ = !tick_values_manual;
  touch();
  return *this;
}

const std::vector<double>& axis::tick_values() const { return tick_values_; }

class axis& axis::tick_values(const std::vector<double>& tick_values) {
  tick_values_ = tick_values;
  tick_values_automatic_ = false;
  touch();
  return *this;
}

const std::vector<std::string>& axis::ticklabels() const { return ticklabels_; }

class axis& axis::ticklabels(const std::vector<std::string>& ticklabels) {
  ticklabels_ = ticklabels;
  if (ticklabels.empty()) {
    tick_values({});
  }
  touch();
  return *this;
}

bool axis::ticklabels_mode() const { return ticklabels_mode_; }

class axis& axis::ticklabels_mode(bool ticklabels_mode) {
  ticklabels_mode_ = ticklabels_mode;
  touch();
  return *this;
}

std::string axis::tick_values_string(bool minor_ticks) const {
  std::string r = " (";
  // Future work: use calcticks inside tick_values and
  // ticklabels whenever we call this function
  // regardless of whether the ticks are automatic.
  // This requires keeping preprocessed data in this object.
  // std::vector<double> values = tick_values();
  // std::vector<std::string> labels = ticklabels();
  // And proceed with these labels.
  // We can then always use manual labels in the gnuplot
  // backend, which would make axes more consistent
  // with our format labels and avoid approximation
  // errors in polar plots. It would also be useful
  // for a future opengl backend, where we always
  // need explicit labels.
  for (size_t i = 0; i < tick_values_.size(); ++i) {
    if (i != 0) {
      r += ", ";
    }
    if (ticklabels_mode() && ticklabels().size() > i) {
      r += "\"" + escape(ticklabels_[i]) + "\" ";
    } else {
      // Unfortunately, gnuplot cancels the formatting
      // when we set the tic values explicitly.
      // So if there is no explicit label but there is
      // a format we want to apply, we also need to explicitly
      // create a label according to our format.
      r += "\"" + escape(num2str(tick_values_[i], tick_label_format_)) + "\" ";
    }
    r += num2str(tick_values_[i]);
    if (minor_ticks) {
      r += " 0";
      if (i != tick_values_.size() - 1) {
        double m_tick = (tick_values_[i] + tick_values_[i + 1]) / 2.;
        r += ", " + num2str(m_tick) + " 1";
      }
    }
  }
  r += ")";
  return r;
}

std::string axis::tick_rotate_string() const {
  if (tickangle_ == 0) {
    return " norotate";
  } else {
    return " rotate by " + num2str(tickangle_) + " right";
  }
}

axis::axis_scale axis::scale() const { return scale_; }

class axis& axis::scale(axis::axis_scale scale) {
  if (scale != scale_) {
    scale_ = scale;
    touch();
  }
  return *this;
}

bool axis::visible() const { return visible_; }

class axis& axis::visible(bool visible) {
  visible_ = visible;
  touch();
  return *this;
}

float axis::tick_length() const { return tick_length_; }

class axis& axis::tick_length(float tick_length) {
  tick_length_ = tick_length;
  touch();
  return *this;
}

bool axis::zero_axis() const { return zero_axis_; }

class axis& axis::zero_axis(bool zero_axis) {
  zero_axis_ = zero_axis;
  touch();
  return *this;
}

bool axis::geographic() const { return geographic_; }

class axis& axis::geographic(bool geographic) {
  geographic_ = geographic;
  touch();
  return *this;
}

bool axis::on_axis() const { return on_axis_; }

class axis& axis::on_axis(bool on_axis) {
  on_axis_ = on_axis;
  touch();
  return *this;
}

const std::string& axis::label_weight() const { return label_weight_; }

class axis& axis::label_weight(const std::string& label_weight) {
  label_weight_ = label_weight;
  touch();
  return *this;
}

float axis::label_font_size() const { return label_font_size_; }

class axis& axis::label_font_size(float label_font_size) {
  label_font_size_ = label_font_size;
  touch();
  return *this;
}

const color_array& axis::label_color() const { return label_color_; }

class axis& axis::label_color(const color_array& label_color) {
  label_color_ = label_color;
  touch();
  return *this;
}

float axis::tickangle() const { return tickangle_; }

class axis& axis::tickangle(float tickangle) {
  tickangle_ = tickangle;
  touch();
  return *this;
}

}  // namespace matplot