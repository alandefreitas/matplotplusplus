//
// Created by Alan Freitas on 26/08/20.
//

#ifndef MATPLOTPLUSPLUS_BACKEND_REGISTRY_H
#define MATPLOTPLUSPLUS_BACKEND_REGISTRY_H

#include <matplot/backend/backend_interface.h>
#include <matplot/backend/gnuplot.h>

namespace matplot {
template <class BACKEND_TYPE>
std::shared_ptr<backend::backend_interface> create_backend() {
  if constexpr (std::is_base_of_v<backend::backend_interface, BACKEND_TYPE>) {
    std::shared_ptr<BACKEND_TYPE> dp = std::make_shared<BACKEND_TYPE>();
    std::shared_ptr<backend::backend_interface> bp =
        std::dynamic_pointer_cast<backend::backend_interface>(dp);
    return bp;
  } else {
    throw std::invalid_argument(
        std::string("The class ") + typeid(BACKEND_TYPE).name() +
        " does not derive from backend::backend_interface");
  }
}

std::shared_ptr<backend::backend_interface> create_default_backend();
}  // namespace matplot

#endif  // MATPLOTPLUSPLUS_BACKEND_REGISTRY_H
