//
// Created by Alan Freitas on 26/08/20.
//

#include "opengl_3.h"
#include <future>
#include <matplot/util/common.h>
#include <thread>

namespace matplot::backend {

    opengl_3::opengl_3() {
        // Initialize
        if (!glfwInit()) {
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // Initialization hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Create window
        window_ = glfwCreateWindow(default_screen_width, default_screen_height,
                                   "Figure 1", nullptr, nullptr);
        if (window_ == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        // Set window as context
        glfwMakeContextCurrent(window_);

        // Set callback when we resize the window
        glfwSetFramebufferSizeCallback(window_,
                                       opengl_3::framebuffer_size_callback);

        // Load OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    opengl_3::~opengl_3() {
        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
    }

    bool opengl_3::is_interactive() { return true; }

    const std::string &opengl_3::output() {
        throw std::logic_error("output not implemented yet");
    }

    const std::string &opengl_3::output_format() {
        throw std::logic_error("output_format not implemented yet");
    }

    bool opengl_3::output(const std::string &filename) {
        throw std::logic_error("output not implemented yet");
    }

    bool opengl_3::output(const std::string &filename,
                          const std::string &file_format) {
        throw std::logic_error("output not implemented yet");
    }

    unsigned int opengl_3::width() {
        throw std::logic_error("width not implemented yet");
    }

    unsigned int opengl_3::height() {
        throw std::logic_error("height not implemented yet");
    }

    void opengl_3::width(unsigned int new_width) {
        throw std::logic_error("width not implemented yet");
    }

    void opengl_3::height(unsigned int new_height) {
        throw std::logic_error("height not implemented yet");
    }

    unsigned int opengl_3::position_x() {
        throw std::logic_error("position_x not implemented yet");
    }

    unsigned int opengl_3::position_y() {
        throw std::logic_error("position_y not implemented yet");
    }

    void opengl_3::position_x(unsigned int new_position_x) {
        throw std::logic_error("position_x not implemented yet");
    }

    void opengl_3::position_y(unsigned int new_position_y) {
        throw std::logic_error("position_y not implemented yet");
    }

    void opengl_3::new_frame() {
        throw std::logic_error("new_frame not implemented yet");
    }

    bool opengl_3::render_data() {
        // This is only a demonstration.
        // Instead of entering into this loop, the render_data
        // function should only update the data we should render in a
        // loop that would be already running since we created the backend.
        // The loop should be running in another thread.
        while (!glfwWindowShouldClose(window_)) {
            // Check if there is any input to process
            process_input(window_);

            // Render the window
            // The drawing commands should go here
            auto bg = background_color();
            glClearColor(bg[0], bg[1], bg[2], bg[3]);
            glClear(GL_COLOR_BUFFER_BIT);
            // ...

            // glfw: swap buffers and poll IO events (keys pressed/released,
            // mouse moved etc.)
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
        return true;
    }

    void opengl_3::wait() { backend_interface::wait(); }

    bool opengl_3::supports_fonts() {
        throw std::logic_error("supports_fonts not implemented yet");
    }

    void opengl_3::draw_path(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &z) {
        throw std::logic_error("draw_path not implemented yet");
    }

    void opengl_3::draw_markers(const std::vector<double> &x,
                                const std::vector<double> &y,
                                const std::vector<double> &z) {
        throw std::logic_error("draw_markers not implemented yet");
    }

    void opengl_3::draw_text(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &z) {
        throw std::logic_error("draw_text not implemented yet");
    }

    void opengl_3::draw_image(const std::vector<std::vector<double>> &x,
                              const std::vector<std::vector<double>> &y,
                              const std::vector<std::vector<double>> &z) {
        throw std::logic_error("draw_image not implemented yet");
    }

    void opengl_3::draw_triangle(const std::vector<double> &x,
                                 const std::vector<double> &y,
                                 const std::vector<double> &z) {
        throw std::logic_error("draw_triangle not implemented yet");
    }

    void opengl_3::process_input(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void opengl_3::framebuffer_size_callback(GLFWwindow *window, int width,
                                             int height) {
        // Make sure the viewport matches the new window dimensions;
        // Note that width and height will be significantly larger than
        // specified on retina displays.
        glViewport(0, 0, width, height);
    }

} // namespace matplot::backend