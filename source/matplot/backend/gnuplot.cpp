//
// Created by Alan Freitas on 26/08/20.
//

#include "gnuplot.h"

#include <cstdlib>
#include <iostream>
#include <matplot/util/common.h>
#include <matplot/util/popen.h>
#include <regex>
#include <thread>

#ifdef __has_include
#if __has_include(<filesystem>)
#include <filesystem>
#else
#define CXX_NO_FILESYSTEM
#endif
#else
#define CXX_NO_FILESYSTEM
#endif

#ifdef MATPLOT_HAS_FBUFSIZE

#include <stdio_ext.h>

static size_t gnuplot_pipe_capacity(FILE *f) {
    size_t sz = __fbufsize(f);
    return sz != 0 ? sz : matplot::backend::gnuplot::pipe_capacity_worst_case;
}

#else

static size_t gnuplot_pipe_capacity(FILE *) {
    return matplot::backend::gnuplot::pipe_capacity_worst_case;
}

#endif // MATPLOT_HAS_FBUFSIZE

namespace matplot::backend {
    bool gnuplot::consumes_gnuplot_commands() { return true; }

    gnuplot::gnuplot() {
        // 1st option: terminal in GNUTERM environment variable
#if defined(_MSC_VER)
        char *environment_terminal;
        size_t len;
        errno_t err = _dupenv_s(&environment_terminal, &len, "GNUTERM");
        const bool env_found = err == 0 && environment_terminal != nullptr;
#else
        char *environment_terminal = std::getenv("GNUTERM");
        bool env_found = environment_terminal != nullptr;
#endif
        if (env_found) {
            if (terminal_is_available(environment_terminal)) {
                terminal_ = environment_terminal;
            }
#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__)
        } else if (terminal_is_available("wxt")) {
            // 2nd option: wxt on windows, even if not default
            terminal_ = "wxt";
#endif
        } else if (terminal_is_available("qt")) {
            // 3rd option: qt
            terminal_ = "qt";
        } else {
            // 4rd option: default terminal type
            terminal_ = default_terminal_type();
        }

        // Open the gnuplot pipe_
        if constexpr (windows_should_persist_by_default) {
            pipe_ = POPEN("gnuplot --persist", "w");
        } else {
            pipe_ = POPEN("gnuplot", "w");
        }

        // Check if everything is OK
        if (!pipe_) {
            std::cerr << "Opening the gnuplot pipe_ failed!" << std::endl;
            std::cerr
                << "Please install gnuplot 5.2.6+: http://www.gnuplot.info"
                << std::endl;
        }
    }

    gnuplot::~gnuplot() {
        if constexpr (dont_let_it_close_too_fast) {
            auto time_since_last_flush =
                std::chrono::high_resolution_clock::now() - last_flush_;
            if (time_since_last_flush < std::chrono::seconds(5)) {
                std::this_thread::sleep_for(std::chrono::seconds(5) -
                                            time_since_last_flush);
            }
        }
        flush_commands();
        run_command("exit");
        flush_commands();
        if (pipe_) {
            PCLOSE(pipe_);
        }
    }

    bool gnuplot::is_interactive() { return output_.empty(); }

    const std::string &gnuplot::output() { return output_; }

    const std::string &gnuplot::output_format() { return terminal_; }

#ifdef STRING_VIEW_CONSTEXPR_BUG
#define SV_CONSTEXPR
#else
#define SV_CONSTEXPR constexpr
#endif

    bool gnuplot::output(const std::string &filename) {
        if (filename.empty()) {
            output_ = filename;
            terminal_ = default_terminal_type();
            return true;
        }

        // look at the extension
#ifndef CXX_NO_FILESYSTEM
        namespace fs = std::filesystem;
        fs::path p{filename};
        std::string ext = p.extension().string();
#else
        std::string ext = filename.substr(filename.find_last_of('.'));
#endif

        // check terminal for that extension
        SV_CONSTEXPR auto exts = extension_terminal();
        auto it = std::find_if(exts.begin(), exts.end(),
                               [&](const auto &e) { return e.first == ext; });

        // if there is a terminal
        if (it != exts.end()) {
            // terminal name is the file format
            output(filename, std::string(it->second));
            return true;
        } else {
            // set file format to dumb
            std::cerr << "No gnuplot terminal for " << ext << " files"
                      << std::endl;
            std::cerr << "Setting terminal to \"dumb\"" << std::endl;
            output(filename, "dumb");
            return false;
        }
    }

    bool gnuplot::output(const std::string &filename,
                         const std::string &format) {
        // If filename is empty - format should be interactive
        // We don't check extension_terminal because that's only
        // for non-interactive terminal
        if (filename.empty()) {
            output_ = filename;
            terminal_ = format;
            return true;
        }

        // Check if file format is valid
        SV_CONSTEXPR auto exts = extension_terminal();
        auto it = std::find_if(exts.begin(), exts.end(), [&](const auto &e) {
            return e.second == format;
        });

        if (it == exts.end()) {
            std::cerr << format << " format does not exist for gnuplot backend"
                      << std::endl;
            return false;
        }

        // Create file if it does not exist
#ifndef CXX_NO_FILESYSTEM
        namespace fs = std::filesystem;
        fs::path p{filename};
        if (!p.parent_path().empty() && !fs::exists(p.parent_path())) {
            fs::create_directory(p.parent_path());
            if (!fs::exists(p.parent_path())) {
                std::cerr << "Could not find or create " << p.parent_path()
                          << std::endl;
                return false;
            }
        }
#endif

        output_ = filename;
        terminal_ = format;

        // Append extension if needed
#ifndef CXX_NO_FILESYSTEM
        std::string ext = p.extension().string();
#else
        std::string ext = filename.substr(filename.find_last_of('.'));
#endif
        if (ext.empty()) {
            output_ += it->first;
        }
        return true;
    }

    unsigned int gnuplot::width() { return position_[2]; }

    unsigned int gnuplot::height() { return position_[3]; }

    void gnuplot::width(unsigned int new_width) {
        position_[2] = new_width;
        if (terminal_has_position_option(terminal_)) {
            run_command("set terminal " + terminal_ + " position " +
                        num2str(position_[0]) + "," + num2str(position_[1]));
        }
        if (terminal_ == "dumb") {
            run_command("set terminal dumb " + num2str(position_[2]) + " " +
                        num2str(position_[3]));
        } else {
            if (terminal_has_size_option(terminal_)) {
                run_command("set terminal " + terminal_ + " size " +
                            num2str(position_[2]) + "," +
                            num2str(position_[3]));
            }
        }
    }

    void gnuplot::height(unsigned int new_height) {
        position_[3] = new_height;
        if (terminal_has_position_option(terminal_)) {
            run_command("set terminal " + terminal_ + " position " +
                        num2str(position_[0]) + "," + num2str(position_[1]));
        }
        if (terminal_ == "dumb") {
            run_command("set terminal dumb " + num2str(position_[2]) + " " +
                        num2str(position_[3]));
        } else {
            if (terminal_has_size_option(terminal_)) {
                run_command("set terminal " + terminal_ + " size " +
                            num2str(position_[2]) + "," +
                            num2str(position_[3]));
            }
        }
    }

    unsigned int gnuplot::position_x() { return position_[0]; }

    unsigned int gnuplot::position_y() { return position_[1]; }

    void gnuplot::position_x(unsigned int new_position_x) {
        position_[0] = new_position_x;
        if (terminal_has_position_option(terminal_)) {
            run_command("set terminal " + terminal_ + " position " +
                        num2str(position_[0]) + "," + num2str(position_[1]));
        }
        if (terminal_has_size_option(terminal_)) {
            run_command("set terminal " + terminal_ + " size " +
                        num2str(position_[2]) + "," + num2str(position_[3]));
        }
    }

    void gnuplot::position_y(unsigned int new_position_y) {
        position_[1] = new_position_y;
        if (terminal_has_position_option(terminal_)) {
            run_command("set terminal " + terminal_ + " position " +
                        num2str(position_[0]) + "," + num2str(position_[1]));
        }
        if (terminal_has_size_option(terminal_)) {
            run_command("set terminal " + terminal_ + " size " +
                        num2str(position_[2]) + "," + num2str(position_[3]));
        }
    }

    bool gnuplot::new_frame() {
        // always accept starting a new frame
        return true;
    }

    bool gnuplot::render_data() { return flush_commands(); }

    bool gnuplot::flush_commands() {
        if constexpr (dont_let_it_close_too_fast) {
            last_flush_ = std::chrono::high_resolution_clock::now();
        }
        fputs("\n", pipe_);
        fflush(pipe_);
        if constexpr (trace_commands) {
            std::cout << "\n\n\n\n" << std::endl;
        }
        return true;
    }

    bool gnuplot::supports_fonts() {
        return terminal_has_font_option(terminal_);
    }

    void gnuplot::run_command(const std::string &command) {
        if (!pipe_) {
            return;
        }
        size_t pipe_capacity = gnuplot_pipe_capacity(pipe_);
        if (command.size() + bytes_in_pipe_ > pipe_capacity) {
            flush_commands();
            bytes_in_pipe_ = 0;
        }
        if (!command.empty()) {
            fputs(command.c_str(), pipe_);
        }
        // fputs("; ", pipe_);
        fputs("\n", pipe_);
        bytes_in_pipe_ += command.size();
        if constexpr (trace_commands) {
            std::cout << command << std::endl;
        }
    }

    void gnuplot::include_comment(const std::string &comment) {
        if (include_comments_) {
            run_command("# " + comment);
        }
    }

    std::string gnuplot::default_terminal_type() {
        static std::string terminal_type;
        const bool dont_know_term_type = terminal_type.empty();
        if (dont_know_term_type) {
            terminal_type =
                run_and_get_output("gnuplot -e \"show terminal\" 2>&1");
            terminal_type = std::regex_replace(
                terminal_type, std::regex("[^]*terminal type is ([^ ]+)[^]*"),
                "$1");
            const bool still_dont_know_term_type = terminal_type.empty();
            if (still_dont_know_term_type) {
                terminal_type = "qt";
            }
        }
        return terminal_type;
    }

    bool gnuplot::terminal_is_available(std::string_view term) {
        std::string msg =
            run_and_get_output("gnuplot -e \"set terminal " +
                               std::string(term.data()) + "\" 2>&1");
        return msg.empty();
    }

    std::tuple<int, int, int> gnuplot::gnuplot_version() {
        static std::tuple<int, int, int> version{0, 0, 0};
        const bool dont_know_gnuplot_version_yet =
            version == std::tuple<int, int, int>({0, 0, 0});
        if (dont_know_gnuplot_version_yet) {
            std::string version_str =
                run_and_get_output("gnuplot --version 2>&1");
            std::string version_major = std::regex_replace(
                version_str,
                std::regex("[^]*gnuplot (\\d+)\\.\\d+ patchlevel \\d+ *"),
                "$1");
            std::string version_minor = std::regex_replace(
                version_str,
                std::regex("[^]*gnuplot \\d+\\.(\\d+) patchlevel \\d+ *"),
                "$1");
            std::string version_patch = std::regex_replace(
                version_str,
                std::regex("[^]*gnuplot \\d+\\.\\d+ patchlevel (\\d+) *"),
                "$1");
            try {
                std::get<0>(version) = std::stoi(version_major);
            } catch (...) {
                std::get<0>(version) = 0;
            }
            try {
                std::get<1>(version) = std::stoi(version_minor);
            } catch (...) {
                std::get<1>(version) = 0;
            }
            try {
                std::get<2>(version) = std::stoi(version_patch);
            } catch (...) {
                std::get<2>(version) = 0;
            }
            const bool still_dont_know_gnuplot_version =
                version == std::tuple<int, int, int>({0, 0, 0});
            if (still_dont_know_gnuplot_version) {
                // assume it's 5.2.6 by convention
                version = std::tuple<int, int, int>({5, 2, 6});
            }
        }
        return version;
    }

    bool gnuplot::terminal_has_title_option(const std::string &t) {
        SV_CONSTEXPR std::string_view whitelist[] = {
            "qt", "aqua", "caca", "canvas", "windows", "wxt", "x11"};
        return std::find(std::begin(whitelist), std::end(whitelist), t) !=
               std::end(whitelist);
    }

    bool gnuplot::terminal_has_size_option(const std::string &t) {
        // Terminals that have the size option *in the way we expect it to work*
        // This includes only the size option with {width, height} and not
        // the size option for cropping or scaling
        SV_CONSTEXPR std::string_view whitelist[] = {
            "qt",       "aqua",    "caca", "canvas", "eepic",    "emf",
            "gif",      "jpeg",    "pbm",  "png",    "pngcairo", "sixelgd",
            "tkcanvas", "windows", "wxt",  "svg"};
        return std::find(std::begin(whitelist), std::end(whitelist), t) !=
               std::end(whitelist);
    }

    bool gnuplot::terminal_has_position_option(const std::string &t) {
        SV_CONSTEXPR std::string_view whitelist[] = {"qt", "windows", "wxt"};
        return std::find(std::begin(whitelist), std::end(whitelist), t) !=
               std::end(whitelist);
    }

    bool gnuplot::terminal_has_enhanced_option(const std::string &t) {
        SV_CONSTEXPR std::string_view whitelist[] = {
            "canvas",     "postscript", "qt",       "aqua",     "caca",
            "canvas",     "dumb",       "emf",      "enhanced", "jpeg",
            "pdf",        "pdfcairo",   "pm",       "png",      "pngcairo",
            "postscript", "sixelgd",    "tkcanvas", "windows",  "wxt",
            "x11",        "ext",        "wxt"};
        return std::find(std::begin(whitelist), std::end(whitelist), t) !=
               std::end(whitelist);
    }

    bool gnuplot::terminal_has_color_option(const std::string &t) {
        SV_CONSTEXPR std::string_view whitelist[] = {
            "postscript", "aifm",     "caca",     "cairolatex", "context",
            "corel",      "eepic",    "emf",      "epscairo",   "epslatex",
            "fig",        "lua tikz", "mif",      "mp",         "pbm",
            "pdf",        "pdfcairo", "pngcairo", "postscript", "pslatex",
            "pstex",      "tgif",     "windows"};
        return std::find(std::begin(whitelist), std::end(whitelist), t) !=
               std::end(whitelist);
    }

    bool gnuplot::terminal_has_font_option(const std::string &t) {
        // This includes terminals that don't have the font option
        // and terminals for which we want to use only the default fonts
        // We prefer a blacklist because it's better to get a warning
        // in a false positive than remove the fonts in a false negative.
        SV_CONSTEXPR std::string_view blacklist[] = {
            "dxf",      "eepic",   "emtex",   "hpgl",    "latex",
            "mf",       "pcl5",    "pslatex", "pstex",   "pstricks",
            "qms",      "tek40xx", "tek410x", "texdraw", "tkcanvas",
            "vttek",    "xterm",   "jpeg",    "dumb",    "tpic",
            "sixelgd",  "png",     "lua",     "jpeg",    "epscairo",
            "epslatex", "canvas",  "dumb",    "dxy800a", "emxvga",
            "pdfcairo", "cgi",     "gif",     "gpic",    "grass",
            "hp2623a",  "hp2648",  "hp500c",  "hpgl",    "pcl5",
            "hpljii",   "hppj",    "imagen",  "linux",   "lua",
            "lua tikz", "mf",      "mif",     "mp",      "pbm",
            "pm",       "qms",     "regis",   "svga",    "texdraw",
            "tikz",     "tpic",    "vgagl",   "vws",     "pdf"};
        return std::find(std::begin(blacklist), std::end(blacklist), t) ==
               std::end(blacklist);
    }

} // namespace matplot::backend
