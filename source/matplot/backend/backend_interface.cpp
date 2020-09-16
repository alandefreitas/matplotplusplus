//
// Created by Alan Freitas on 26/08/20.
//

#include "backend_interface.h"
#include <matplot/core/figure_type.h>
#include <matplot/util/common.h>

namespace matplot::backend {
    bool backend_interface::consumes_gnuplot_commands() { return false; }

    bool backend_interface::is_interactive() { return true; }

    const std::string &backend_interface::output() {
        const static std::string r{};
        return r;
    }

    const std::string &backend_interface::output_format() {
        const static std::string r{};
        return r;
    }

    bool
    backend_interface::output([[maybe_unused]] const std::string &filename) {
        throw std::logic_error(
            "There is no function to set the output in this backend yet. Maybe "
            "this backend does not support non-interactive mode.");
    }

    bool
    backend_interface::output([[maybe_unused]] const std::string &filename,
                              [[maybe_unused]] const std::string &file_format) {
        throw std::logic_error(
            "There is no function to set the output in this backend yet. Maybe "
            "this backend does not support non-interactive mode.");
    }

    unsigned int backend_interface::width() { return 560; }

    unsigned int backend_interface::height() { return 420; }

    void backend_interface::width([[maybe_unused]] unsigned int new_width) {
        throw std::logic_error(
            "There is no function to set the width in this backend yet");
    }

    void backend_interface::height([[maybe_unused]] unsigned int new_height) {
        throw std::logic_error(
            "There is no function to set the height in this backend yet");
    }

    bool backend_interface::new_frame() {
        return true;
    }

    bool backend_interface::render_data() {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to render_data in this backend yet");
        } else {
            throw std::logic_error(
                "This backend has no function to flush commands yet");
        }
    }

    void backend_interface::draw_path(
        [[maybe_unused]] const std::vector<double> &x,
        [[maybe_unused]] const std::vector<double> &y,
        [[maybe_unused]] const std::array<float, 4> &color) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_path in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_path "
                                   "because it is based on gnuplot commands");
        }
    }

    void backend_interface::draw_markers(
        [[maybe_unused]] const std::vector<double> &x,
        [[maybe_unused]] const std::vector<double> &y,
        [[maybe_unused]] const std::vector<double> &z) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_markers in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_markers "
                                   "because it is based on gnuplot commands");
        }
    }

    void backend_interface::draw_text(
        [[maybe_unused]] const std::vector<double> &x,
        [[maybe_unused]] const std::vector<double> &y,
        [[maybe_unused]] const std::vector<double> &z) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_text in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_text "
                                   "because it is based on gnuplot commands");
        }
    }

    void backend_interface::draw_image(
        [[maybe_unused]] const std::vector<std::vector<double>> &x,
        [[maybe_unused]] const std::vector<std::vector<double>> &y,
        [[maybe_unused]] const std::vector<std::vector<double>> &z) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_image in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_image "
                                   "because it is based on gnuplot commands");
        }
    }

    void backend_interface::draw_triangle(
        [[maybe_unused]] const std::vector<double> &x,
        [[maybe_unused]] const std::vector<double> &y,
        [[maybe_unused]] const std::vector<double> &z) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_triangle in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_triangle "
                                   "because it is based on gnuplot commands");
        }
    }

    void
    backend_interface::run_command([[maybe_unused]] const std::string &text) {
        if (consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to run_command in this backend yet");
        } else {
            throw std::logic_error(
                "This backend has no function to run_command because it is not "
                "based on gnuplot commands");
        }
    }

    void backend_interface::include_comment(
        [[maybe_unused]] const std::string &text) {
        if (consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to include_comment in this backend yet");
        } else {
            throw std::logic_error(
                "This backend has no function to include_comment because it is "
                "not based on gnuplot commands");
        }
    }

    void backend_interface::show(class matplot::figure_type *f) {
        // The default implementation waits for the user to interact with the
        // console. In interactive backends we expect this to start a render
        // loop that will stop only when the user closes the window.
        f->draw();
        matplot::wait();
    }

    bool backend_interface::supports_fonts() { return false; }

    unsigned int backend_interface::position_x() { return 680; }

    unsigned int backend_interface::position_y() { return 558; }

    void backend_interface::position_x(
        [[maybe_unused]] unsigned int new_position_x) {}

    void backend_interface::position_y(
        [[maybe_unused]] unsigned int new_position_y) {}

    void backend_interface::draw_background(
        [[maybe_unused]] const std::array<float, 4> &color) {}

    void backend_interface::draw_rectangle(
        [[maybe_unused]] const double x1, [[maybe_unused]] const double x2,
        [[maybe_unused]] const double y1, [[maybe_unused]] const double y2,
        [[maybe_unused]] const std::array<float, 4> &color) {
        if (!consumes_gnuplot_commands()) {
            throw std::logic_error(
                "There is no function to draw_rectangle in this backend yet");
        } else {
            throw std::logic_error("This backend has no function draw_triangle "
                                   "because it is based on gnuplot commands");
        }
    }

    bool backend_interface::should_close() {
        // by default, we assume backends simply don't have this feature
        return false;
    }

    void
    backend_interface::window_title([[maybe_unused]] const std::string &title) {
    }

    std::string backend_interface::window_title() {
        return "";
    }

} // namespace matplot::backend