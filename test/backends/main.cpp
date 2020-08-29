//
// Created by Alan Freitas on 26/08/20.
//

#include <matplot/matplot.h>
#include <matplot/backend/opengl_3.h>

int main() {
    using namespace matplot;

    // This is a demonstration of how we could create a new backend
    // - This example backend will not draw the plots yet
    // - See the opengl_3.h file for more information on how
    // you can prepare a backend that will draw vertices
    // - See figure::draw() for more information on how you can
    // make the matplot objects send vertices to these
    // backends
    auto opengl3 = create_backend<backend::opengl_3>();
    auto f = figure(true);
    f->backend(opengl3);
    f->draw();
    return 0;
}