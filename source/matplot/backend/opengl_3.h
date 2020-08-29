//
// Created by Alan Freitas on 26/08/20.
//

#ifndef MATPLOTPLUSPLUS_OPENGL_3_H
#define MATPLOTPLUSPLUS_OPENGL_3_H

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages on
 * Apple */
/* What the hell is Apple thinking? */
#define GL_SILENCE_DEPRECATION
#endif

/// Because OpenGL is only really a standard/specification it is up to
/// the driver manufacturer to implement the specification to a driver
/// that the specific graphics card supports. Since there are many
/// different versions of OpenGL drivers, the location of most of its
/// functions is not known at compile-time and needs to be queried at
/// run-time. GLAD is an open source library that manages all that
/// cumbersome work we talked about.
/// \see https://learnopengl.com/Getting-started/Creating-a-window
#include <glad/glad.h>

/// GLFW gives us the bare necessities required for rendering goodies
/// to the screen. It allows us to create an OpenGL context, define
/// window parameters, and handle user input
/// \see https://learnopengl.com/Getting-started/Creating-a-window
#include <GLFW/glfw3.h>

#include <matplot/backend/backend_interface.h>

namespace matplot::backend {
    class opengl_3 : public backend_interface {
      public:
        opengl_3();
        ~opengl_3();

      public:
        bool is_interactive() override;
        const std::string &output() override;
        const std::string &output_format() override;
        bool output(const std::string &filename) override;
        bool output(const std::string &filename,
                    const std::string &file_format) override;
        unsigned int width() override;
        unsigned int height() override;
        void width(unsigned int new_width) override;
        void height(unsigned int new_height) override;
        unsigned int position_x() override;
        unsigned int position_y() override;
        void position_x(unsigned int new_position_x) override;
        void position_y(unsigned int new_position_y) override;
        void new_frame() override;
        // This is only a demonstration that is blocking the main thread.
        // Because glfw does not work well with multiple threads,
        // the challenge is to run the rendering loop in another thread
        // while we see this function only to update what should be rendered.
        bool render_data() override;
        void wait() override;
        bool supports_fonts() override;

      public:
        // These functions should buffer these commands
        // When we call render_data, we start rendering something
        // else on the rendering thread
        void draw_path(const std::vector<double> &x,
                       const std::vector<double> &y,
                       const std::vector<double> &z = {}) override;
        void draw_markers(const std::vector<double> &x,
                          const std::vector<double> &y,
                          const std::vector<double> &z = {}) override;
        void draw_text(const std::vector<double> &x,
                       const std::vector<double> &y,
                       const std::vector<double> &z = {}) override;
        void
        draw_image(const std::vector<std::vector<double>> &x,
                   const std::vector<std::vector<double>> &y,
                   const std::vector<std::vector<double>> &z = {}) override;
        void draw_triangle(const std::vector<double> &x,
                           const std::vector<double> &y,
                           const std::vector<double> &z = {}) override;

      private:
        const unsigned int default_screen_width = 560;
        const unsigned int default_screen_height = 420;

        /// \brief glfw: whenever the window size changed (by OS or user resize)
        /// this callback function executes
        static void framebuffer_size_callback(GLFWwindow *window, int width,
                                              int height);

        /// \brief Process all input
        /// Query GLFW whether relevant keys are pressed/released
        /// this frame and react accordingly
        void process_input(GLFWwindow *window);

      private:
        GLFWwindow *window_{nullptr};
    };
} // namespace matplot::backend

#endif // MATPLOTPLUSPLUS_OPENGL_3_H
