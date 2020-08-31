//
// Created by Alan Freitas on 26/08/20.
//

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <matplot/backend/opengl_3.h>
#include <matplot/matplot.h>

// The opengl backend can be used inside the render loop
// to generate plots. This is still very experimental.
//
// This is a demonstration of how we could create a new backend
// - This example backend will not draw the plots yet
// - See the opengl_3.h file for more information on how
//   you can prepare a backend that will draw vertices
// - See figure::draw() for more information on how you can
//   make the matplot objects send vertices to these
//   backends
int main() {
    using namespace matplot;

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
    GLFWwindow *window =
        glfwCreateWindow(backend::opengl_3::default_screen_width,
                         backend::opengl_3::default_screen_height,
                         "My Figure", nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Set window as context
    glfwMakeContextCurrent(window);

    // Set callback when we resize the window
    glfwSetFramebufferSizeCallback(window, backend::opengl_3::framebuffer_size_callback);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Create figure with backend
    auto opengl3 = create_backend<backend::opengl_3>();
    auto f = figure(true);
    f->backend(opengl3);
    auto ax = f->current_axes();
    ax->xlim({0.,2. * pi});
    ax->ylim({-2.,2.});

    // Start rendering
    while (!glfwWindowShouldClose(window)) {
        // Check if there is any input to process
        backend::opengl_3::process_input(window);

        // Create plots
        float timeValue = glfwGetTime();
        std::vector<double> x = linspace(0., 2. * pi);
        std::vector<double> y = transform(x, [&](auto x) { return sin(x + timeValue); });
        ax->hold(off);
        ax->plot(x, y, "-o");
        ax->hold(on);
        ax->plot(x, transform(y, [](auto y) { return -y; }), "--xr");
        ax->plot(x, transform(x, [](auto x) { return x / pi - 1.; }), "-:gs");
        ax->plot({1.0, 0.7, 0.4, 0.0, -0.4, -0.7, -1}, "k");

        // Draw the figure
        f->draw();

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}