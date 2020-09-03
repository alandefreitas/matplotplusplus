//
// Created by Alan Freitas on 26/08/20.
//

#include "opengl.h"
#include <matplot/core/figure_type.h>
#include <matplot/util/common.h>

namespace matplot::backend {

    bool opengl::glfw_is_initialized = false;

    opengl::opengl() : opengl_embed(false) {
        if (!glfw_is_initialized) {
            initialize_glfw();
        }

        // Create window
        window_ =
            glfwCreateWindow(backend::opengl::default_screen_width,
                             backend::opengl::default_screen_height,
                             "Figure 1", nullptr, nullptr);

        if (window_ == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        // Set window as context
        glfwMakeContextCurrent(window_);

        // Set callback when we resize the window
        glfwSetFramebufferSizeCallback(window_, backend::opengl_embed::framebuffer_size_callback);

        // Load OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // Create shaders only now that we have a window
        opengl_embed::create_shaders();
    }

    opengl::~opengl() {
        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
    }

    bool opengl::new_frame() {
        if (should_close()) {
            return false;
        }
        this_window_in_context_.lock();
        glfwMakeContextCurrent(window_);
        // Check if there is any input to process
        backend::opengl_embed::process_input(window_);
        return opengl_embed::new_frame();
    }

    bool opengl::render_data() {
        bool r = opengl_embed::render_data();
        // Swap buffers and poll IO events
        glfwSwapBuffers(window_);
        glfwPollEvents();
        this_window_in_context_.unlock();
        return r;
    }

    bool opengl::should_close() {
        return glfwWindowShouldClose(window_);
    }

    void opengl::show(class matplot::figure_type *f) {
        while (!this->should_close()) {
            f->draw();
        }
    }

    void opengl::initialize_glfw() {
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

        glfw_is_initialized = true;
    }
    double opengl::get_time() {
        return glfwGetTime();
    }

    void opengl::window_title(const std::string& title) {
        if (title != window_title_) {
            window_title_ = title;
            glfwSetWindowTitle(window_,title.c_str());
        }
    }

    std::string opengl::window_title() {
        return window_title_;
    }

} // namespace matplot::backend