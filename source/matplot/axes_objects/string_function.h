//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_STRING_FUNCTION_H
#define MATPLOTPLUSPLUS_STRING_FUNCTION_H

#include <matplot/axes_objects/line.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/handle_types.h>
#include <string>

namespace matplot {
    class string_function : public line {
      public:
        explicit string_function(class axes *parent);
        string_function(class axes *parent, const std::string &equation,
                        const std::string &line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        string_function(const axes_handle &parent, Args... args)
            : string_function(parent.get(), args...) {}

      public:
        std::string plot_string() override;
        std::string data_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public:
        const std::string &equation() const;
        class string_function &equation(const std::string &equation);

      private:
        std::string equation_;
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_STRING_FUNCTION_H
