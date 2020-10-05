//
// Created by Alan Freitas on 03/09/20.
//

#include <map>
#include <matplot/core/figure_registry.h>

namespace matplot {
    namespace detail {
        /// Map with all figure handles there are
        std::map<size_t, figure_handle> &global_figure_handles() {
            static std::map<size_t, figure_handle> registry;
            return registry;
        }

        /// Global handle to the figure considered the current figure
        figure_handle &current_figure_handle() {
            static figure_handle cfh;
            return cfh;
        }

        void set_current_figure_handle(figure_handle &h) {
            current_figure_handle() = h;
        }

        /// Register a figure handler with a given index
        void register_figure_handle(size_t figure_index,
                                    const figure_handle &h) {
            global_figure_handles()[figure_index] = h;
        }

        /// Find an index for a new handler and register it there
        void register_figure_handle(const figure_handle &h) {
            size_t index_candidate = 1;
            for (const auto &[index, figure_handle] : global_figure_handles()) {
                if (index_candidate != index) {
                    break;
                }
                ++index_candidate;
            }
            register_figure_handle(index_candidate, h);
        }

        figure_handle register_figure_handle(bool quiet_mode) {
            size_t index_candidate = 1;
            for (const auto &[index, figure_handle] : global_figure_handles()) {
                if (index_candidate != index) {
                    break;
                }
                ++index_candidate;
            }
            figure_handle h = std::make_shared<class figure_type>(
                index_candidate, quiet_mode);
            register_figure_handle(index_candidate, h);
            return h;
        }
    } // namespace detail

    figure_handle figure_no_backend(bool quiet_mode) {
        figure_handle h = detail::register_figure_handle(quiet_mode);
        detail::set_current_figure_handle(h);
        return h;
    }

    figure_handle figure(figure_handle h) {
        detail::set_current_figure_handle(h);
        return h;
    }

    figure_handle figure(class figure_type *h) {
        return figure(figure_handle{h});
    }

    figure_handle gcf() {
        figure_handle h = detail::current_figure_handle();
        if (!h) {
            h = figure();
        }
        return h;
    }

    figure_handle gcf(bool quiet) {
        figure_handle h = gcf();
        h->quiet_mode(quiet);
        return h;
    }

} // namespace matplot