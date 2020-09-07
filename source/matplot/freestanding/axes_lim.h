//
// Created by Alan Freitas on 04/08/20.
//

#ifndef MATPLOTPLUSPLUS_AXES_LIM_H
#define MATPLOTPLUSPLUS_AXES_LIM_H

#include <matplot/core/axes_type.h>

namespace matplot {
    inline std::array<double, 2> xlim() { return gca()->xlim(); }

    inline void xlim(const std::array<double, 2> &lim) { gca()->xlim(lim); }

    inline void xlim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->xlim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void xlim(axes_handle ax, Args&&... args) {
        ax->xlim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void xlim(NotAxesHandle<T> x, Args&&... args) {
        gca()->xlim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> cblim() { return gca()->cblim(); }

    inline void cblim(const std::array<double, 2> &lim) { gca()->cblim(lim); }

    inline void cblim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->cblim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void cblim(axes_handle ax, Args&&... args) {
        ax->cblim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void cblim(NotAxesHandle<T> x, Args&&... args) {
        gca()->cblim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> caxis() { return gca()->cblim(); }

    inline void caxis(const std::array<double, 2> &lim) { gca()->cblim(lim); }

    inline void caxis(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->cblim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void caxis(axes_handle ax, Args&&... args) {
        ax->cblim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void caxis(NotAxesHandle<T> x, Args&&... args) {
        gca()->cblim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> x2lim() { return gca()->x2lim(); }

    inline void x2lim(const std::array<double, 2> &lim) { gca()->x2lim(lim); }

    inline void x2lim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->x2lim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void x2lim(axes_handle ax, Args&&... args) {
        ax->x2lim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void x2lim(NotAxesHandle<T> x, Args&&... args) {
        gca()->x2lim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> ylim() { return gca()->ylim(); }

    inline void ylim(const std::array<double, 2> &lim) { gca()->ylim(lim); }

    inline void ylim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->ylim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void ylim(axes_handle ax, Args&&... args) {
        ax->ylim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void ylim(NotAxesHandle<T> x, Args&&... args) {
        gca()->ylim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> y2lim() { return gca()->y2lim(); }

    inline void y2lim(const std::array<double, 2> &lim) { gca()->y2lim(lim); }

    inline void y2lim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->y2lim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void y2lim(axes_handle ax, Args&&... args) {
        ax->y2lim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void y2lim(NotAxesHandle<T> x, Args&&... args) {
        gca()->y2lim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> zlim() { return gca()->zlim(); }

    inline void zlim(const std::array<double, 2> &lim) { gca()->zlim(lim); }

    inline void zlim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->zlim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void zlim(axes_handle ax, Args&&... args) {
        ax->zlim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void zlim(NotAxesHandle<T> x, Args&&... args) {
        gca()->zlim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> rlim() { return gca()->rlim(); }

    inline void rlim(const std::array<double, 2> &lim) { gca()->rlim(lim); }

    inline void rlim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->rlim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void rlim(axes_handle ax, Args&&... args) {
        ax->rlim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void rlim(NotAxesHandle<T> x, Args&&... args) {
        gca()->rlim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> tlim() { return gca()->tlim(); }

    inline void tlim(const std::array<double, 2> &lim) { gca()->tlim(lim); }

    inline void tlim(std::initializer_list<double> lim) {
        if (lim.size() >= 2) {
            gca()->tlim(to_array<2>(lim));
        }
    }

    template <class T, class... Args> void tlim(axes_handle ax, Args&&... args) {
        ax->tlim(std::forward<Args>(args)...);
    }

    template <class T, class... Args>
    void tlim(NotAxesHandle<T> x, Args&&... args) {
        gca()->tlim(x, std::forward<Args>(args)...);
    }

    inline std::array<double, 2> xlim(axes_handle ax) {
        return ax->xlim();
    }

    inline void xlim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->xlim(lim);
    }

    inline std::array<double, 2> x2lim(axes_handle ax) {
        return ax->x2lim();
    }

    inline void x2lim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->x2lim(lim);
    }

    inline std::array<double, 2> ylim(axes_handle ax) {
        return ax->ylim();
    }

    inline void ylim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->ylim(lim);
    }

    inline std::array<double, 2> y2lim(axes_handle ax) {
        return ax->y2lim();
    }

    inline void y2lim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->y2lim(lim);
    }

    inline std::array<double, 2> zlim(axes_handle ax) {
        return ax->zlim();
    }

    inline void zlim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->zlim(lim);
    }

    inline std::array<double, 2> rlim(axes_handle ax) {
        return ax->rlim();
    }

    inline void rlim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->rlim(lim);
    }

    inline std::array<double, 2> tlim(axes_handle ax) {
        return ax->tlim();
    }

    inline void tlim(axes_handle ax, const std::array<double, 2> &lim) {
        ax->tlim(lim);
    }
} // namespace matplot

#endif // MATPLOTPLUSPLUS_AXES_LIM_H
