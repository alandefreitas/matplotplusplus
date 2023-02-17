#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <regex>

int main() {
    // Set CMake option MATPLOTPP_BUILD_FOR_DOCUMENTATION_IMAGES to ON and
    //       run this executable from the root directory as working
    //       directory to run all examples.
    // If will set 1) the figure object to save the figure when being destroyed
    //         and 2) the wait function to be ignored,
    // This will create .svg and .gif files with all the examples for the documentation
    // Some examples, such as heatmaps and some contours, are better generated
    //     non-programmatically with the qt widgets. We can just replace these
    //     .svgs as we want and this executable will use this svg instead of
    //     running the example.
    using namespace std;
    namespace fs = filesystem;

    // What each relative directory represents
    map<string, string> dir_map = {
            {"line_plot",                        "Line Plots"},
            {"line_plot/plot",                   "Line Plot"},
            {"line_plot/plot3",                  "Line Plot 3D"},
            {"line_plot/stairs",                 "Stairs"},
            {"line_plot/errorbar",               "Error Bars"},
            {"line_plot/area",                   "Area"},
            {"line_plot/loglog",                 "Loglog Plot"},
            {"line_plot/semilogx",               "Semilogx Plot"},
            {"line_plot/semilogy",               "Semilogy Plot"},
            {"line_plot/fplot",                  "Function Plot"},
            {"line_plot/fplot3",                 "Function Plot 3D"},
            {"line_plot/fimplicit",              "Implicit function"},
            {"data_distribution",                "Data Distribution"},
            {"data_distribution/histogram",      "Histogram"},
            {"data_distribution/boxplot",        "Boxplot"},
            {"data_distribution/scatter",        "Scatter Plot"},
            {"data_distribution/scatter3",       "Scatter Plot 3D"},
            {"data_distribution/binscatter",     "Binned Scatter Plots"},
            {"data_distribution/plotmatrix",     "Plot Matrix"},
            {"data_distribution/parallelplot",   "Parallel Coordinates"},
            {"data_distribution/pie",            "Pie Chart"},
            {"data_distribution/heatmap",        "Heatmap"},
            {"data_distribution/wordcloud",      "Word Cloud"},
            {"discrete_data",                    "Discrete Data"},
            {"discrete_data/bar",                "Bar Plot"},
            {"discrete_data/pareto",             "Pareto Chart"},
            {"discrete_data/stem",               "Stem Plot"},
            {"discrete_data/stem3",              "Stem Plot 3D"},
            {"geography",                        "Geography"},
            {"geography/geoplot",                "Geoplot"},
            {"geography/geoscatter",             "Geoscatter Plot"},
            {"geography/geobubble",              "Geobubble"},
            {"geography/geodensityplot",         "Geodensity Plot"},
            {"polar_plots",                      "Polar Plots"},
            {"polar_plots/polarplot",            "Polar Line Plot"},
            {"polar_plots/polarscatter",         "Polar Scatter Plot"},
            {"polar_plots/polarhistogram",       "Polar Histogram"},
            {"polar_plots/compass",              "Compass"},
            {"polar_plots/ezpolar",              "Polar Function"},
            {"contour_plots",                    "Contour Plots"},
            {"contour_plots/contour",            "Contour"},
            {"contour_plots/contourf",           "Filled Contour"},
            {"contour_plots/fcontour",           "Function Contour"},
            {"vector_fields",                    "Vector Fields"},
            {"vector_fields/feather",            "Feather"},
            {"vector_fields/quiver",             "Quiver"},
            {"vector_fields/quiver3",            "Quiver 3D"},
            {"surfaces",                         "Surfaces"},
            {"surfaces/surf",                    "Surface"},
            {"surfaces/surfc",                   "Surface with Contour"},
            {"surfaces/mesh",                    "Mesh"},
            {"surfaces/meshc",                   "Mesh with Contour"},
            {"surfaces/meshz",                   "Mesh with Curtain"},
            {"surfaces/fsurf",                   "Function Surface"},
            {"surfaces/fmesh",                   "Function Mesh"},
            {"surfaces/waterfall",               "Waterfall"},
            {"surfaces/fence",                   "Fence"},
            {"surfaces/ribbon",                  "Ribbon"},
            {"graphs",                           "Graphs"},
            {"graphs/graph",                     "Undirected Graph"},
            {"graphs/digraph",                   "Directed Graph"},
            {"images",                           "Images"},
            {"images/imshow",                    "Image Show"},
            {"images/image",                     "Image Matrix"},
            {"images/imagesc",                   "Scaled Image"},
            {"annotations",                      "Annotations"},
            {"annotations/text",                 "Text"},
            {"annotations/textarrow",            "Text with Arrow"},
            {"annotations/rectangle",            "Rectangle"},
            {"annotations/fill",                 "Filled Polygon"},
            {"annotations/ellipse",              "Ellipse"},
            {"annotations/textbox",              "Textbox"},
            {"annotations/arrow",                "Arrow"},
            {"annotations/line",                 "Line"},
            {"appearance",                       "Appearance"},
            {"appearance/labels",                "Labels"},
            {"appearance/labels/title",          "Title"},
            {"appearance/labels/sgtitle",        "Subplot Title"},
            {"appearance/labels/xlabel",         "X Label"},
            {"appearance/labels/ylabel",         "Y Label"},
            {"appearance/labels/zlabel",         "Z Label"},
            {"appearance/labels/legend",         "Legend"},
            {"appearance/axis",                  "Axis"},
            {"appearance/axis/xlim",             "X Limits"},
            {"appearance/axis/ylim",             "Y Limits"},
            {"appearance/axis/zlim",             "Z Limits"},
            {"appearance/axis/axis",             "Adjust Axis"},
            {"appearance/axis/box",              "Box"},
            {"appearance/grid",                  "Grid"},
            {"appearance/grid/grid",             "Grid"},
            {"appearance/grid/xticks",           "X Ticks"},
            {"appearance/grid/yticks",           "Y Ticks"},
            {"appearance/grid/zticks",           "Z Ticks"},
            {"appearance/grid/xticklabels",      "X Tick Labels"},
            {"appearance/grid/yticklabels",      "Y Tick Labels"},
            {"appearance/grid/xtickformat",      "X Tick Format"},
            {"appearance/grid/ytickformat",      "Y Tick Format"},
            {"appearance/grid/ztickformat",      "Z Tick Format"},
            {"appearance/grid/xtickangle",       "X Tick Angle"},
            {"appearance/grid/ytickangle",       "Y Tick Angle"},
            {"appearance/multiplot",             "Multiplot"},
            {"appearance/multiplot/hold",        "Hold"},
            {"appearance/multiplot/yyaxis",      "YY-axis"},
            {"appearance/multiplot/colororder",  "Color Order"},
            {"appearance/multiplot/subplot",     "Subplots"},
            {"appearance/multiplot/tiledlayout", "Tiled Layout"},
            {"appearance/colormaps",             "Colormaps"},
            {"appearance/colormaps/colormap",    "Colormap"},
            {"appearance/colormaps/colorbar",    "Color Bar"},
            {"appearance/colormaps/rgbplot",     "RGB Plot"},
            {"appearance/camera",                "Camera"},
            {"appearance/camera/view",           "View"},
            {"appearance/camera/lighting",       "Lighting"},
            {"appearance/figure",                "Figure Object"},
            {"appearance/line_spec",             "Line Specs"},
            {"appearance/axes",                  "Axes Object"},
            {"appearance/cla",                   "Clear Axes"},
            {"exporting",                        "Exporting"},
            {"exporting/save",                   "Saving"}
    };

    // Look for root path and build dir
    // The root path has an examples directory and a *build* directory
    fs::path root_path = fs::current_path();
    // Look for build directory
    // The build path has the *build* format
    string build_dir = "cmake-build-debug";
    fs::path build_path = root_path / build_dir;
    if (!fs::exists(build_path)) {
        bool root_has_build = false;
        for(auto& p: fs::directory_iterator(root_path)) {
            if (fs::is_directory(p.path())) {
                if (std::regex_match(p.path().string(), std::regex("*build*"))) {
                    build_path = p.path();
                    root_has_build = true;
                    break;
                }
            }
        }
        if (!root_has_build) {
            cerr << "Root directory has no build directory" << endl;
            return 1;
        }
    }

    // Directories where we have the source files for examples
    fs::path source_examples_path = root_path / "examples";
    if (!fs::exists(source_examples_path)) {
        cerr << "Root directory has no examples directory" << endl;
        return 1;
    }

    // Directory where we will save the images for the documentation
    fs::path documentation_path = root_path / "documentation";

    // Create 3 galleries
    // - Gallery with large images for documentation, linking to source files
    fs::path gallery_path = documentation_path / "GALLERY.md";
    ofstream gallery{gallery_path};
    gallery << "# Complete Gallery\n";
    // - Gallery with thumbnails linking to documentation for README.md
    fs::path thumbnail_gallery_path = root_path / "gallery.md";
    ofstream thumbnail_gallery{thumbnail_gallery_path};
    thumbnail_gallery << "# Thumbnail Gallery\n";
    // - Gallery with one large and rest of thumbnails for examples linking to source files
    fs::path examples_gallery_path = root_path / "examples.md";
    ofstream examples_gallery{examples_gallery_path};
    examples_gallery << "## Examples\n";

    // Variable to keep track of the last directory for which
    // we used to create a header in the galleries
    fs::path last_example_dir = "";
    fs::path last_example_parent_dir = "";
    // The second image in the examples gallery has the string "More examples: "
    bool second_image = false;

    // Read the whole contents of a file
    auto fileread = [](const string& filename) -> string {
        ifstream t(filename);
        if (!t) {
            throw runtime_error("Cannot open the file " + filename);
        }
        string str((istreambuf_iterator<char>(t)),
                        istreambuf_iterator<char>());
        return str;
    };

    // We create a queue of paths to iterate the examples in
    // the order defined by the cmakelists.txt files
    vector<fs::path> path_queue = {source_examples_path};

    // Take the first path from the queue and enqueue
    // the paths to which it makes reference
    auto queue_paths = [&]() {
        // Take first path from the queue
        fs::path p = path_queue.front();
        // Remove this path from the queue
        path_queue.erase(path_queue.begin());
        if (!fs::is_directory(p)) {
            return;
        }
        // Open cmakelists.txt
        string cmake_content = fileread((p/"CMakeLists.txt").string());
        // Get add_subdirectory(...) commands in order identify the file order
        // Enqueue all subdirectories in the order defined by CMakeLists.txt
        vector<fs::path> subdirs = {};
        regex e("add_subdirectory\\((.*)\\)");
        sregex_iterator iter(cmake_content.begin(), cmake_content.end(), e);
        while (iter != sregex_iterator{}) {
            for(unsigned i = 1; i < iter->size(); ++i) {
                fs::path subdir = p;
                subdir /= string((*iter)[i]);
                subdirs.emplace_back(subdir);
            }
            ++iter;
        }
        path_queue.insert(path_queue.begin(), subdirs.begin(), subdirs.end());

        // Enqueue all executable source files in the order defined by CMakeLists.txt
        vector<fs::path> cpps = {};
        e = regex(R"(add_matplot_example\([^ ]+ ([^\) ]+\.cpp)\))");
        iter = sregex_iterator(cmake_content.begin(), cmake_content.end(), e);
        while (iter != sregex_iterator{}) {
            for(unsigned i = 1; i < iter->size(); ++i) {
                fs::path cpp_path = p;
                cpp_path /= string((*iter)[i]);
                if (fs::exists(cpp_path)) {
                    cpps.emplace_back(cpp_path);
                } else {
                    cout << "Source file does not exist: " << cpp_path << endl;
                }
            }
            ++iter;
        }
        path_queue.insert(path_queue.begin(), cpps.begin(), cpps.end());
    };

    // Enqueue the first exeamples paths
    queue_paths();

    // Keep running this while we have examples to include in the galleries
    while (!path_queue.empty()) {
        // Take first path
        fs::path p = path_queue.front();
        // Remove path from queue and enqueue its children
        // directories or source files
        queue_paths();
        
        // If it's a source file, we will include its respective image in the galleries
        const bool is_source_file = !fs::is_directory(p) && p.extension() == ".cpp";
        if (is_source_file) {
            // Print source file
            auto source_filename = p.lexically_proximate(root_path).filename();
            cout << source_filename << " -> ";

            // Print executable file
            fs::path executable = build_path / p.lexically_proximate(root_path);
            executable = executable.parent_path();
            executable /= "example_";
            executable += p.stem();
            auto executable_filename = executable.lexically_proximate(root_path).filename();
            cout << executable_filename << " -> ";

            // Calculate figure destinations
            // Destination directory in documentation
            fs::path figure_destination_dir = documentation_path / p.lexically_proximate(root_path);
            figure_destination_dir = figure_destination_dir.parent_path();
            figure_destination_dir /= p.filename().stem();

            // Destination for svg image
            fs::path figure_destination_svg = figure_destination_dir;
            figure_destination_svg += ".svg";

            // Destination for png image
            fs::path figure_destination_png = figure_destination_dir;
            figure_destination_png += ".png";

            // Destination for png thumbnail
            fs::path figure_destination_thumbnail_png = figure_destination_dir;
            figure_destination_thumbnail_png += "_thumb.png";

            const auto svg_filename = figure_destination_svg.lexically_proximate(root_path).filename();
            cout << svg_filename << '\n';

            // Delete a svg file duplicate if any
            fs::path maybe_svg_duplicate = figure_destination_dir;
            maybe_svg_duplicate += " 2.svg";
            if (fs::exists(maybe_svg_duplicate)) {
                std::cout << "Found duplicate! File " << maybe_svg_duplicate.string() << std::endl;
                fs::remove(maybe_svg_duplicate);
            }

            // Choose one representative figure destination to put in the gallery
            // This will be the svg file or the png file
            // Later on, we will choose the smallest file to put in the gallery
            // and delete the other version.
            fs::path final_figure_destination = figure_destination_svg;

            // Function to update the gallery appending a new image
            const auto update_gallery = [&]() {
                // Append image to the large image gallery
                // - Append HTML anchor
                string anchor = p.stem().string();
                gallery << "<a name=\"" << anchor << "\"></a>";
                // - Append name of source file relative to the repository
                const auto source_file_path = p.lexically_relative(root_path).string();
                gallery << "`" << source_file_path << "`: \n\n";
                // - Append image with link to source file relative to the documentation
                std::string exe_file = executable.filename().stem().string();
                std::string figure_file = final_figure_destination.lexically_proximate(documentation_path).string();
                std::string source_file = p.lexically_relative(documentation_path).string();
                gallery << "[![" << exe_file << "](" << figure_file << ")](" << source_file << ")\n\n";

                // Append image with link to anchor to the thumbnail gallery
                auto thumb_file = figure_destination_thumbnail_png.lexically_proximate(root_path).string();
                auto gallery_file = gallery_path.lexically_relative(root_path).string();
                thumbnail_gallery << "[![" << exe_file << "](" << thumb_file << ")](" << gallery_file << "#" << anchor << ")  ";

                // Append image to the examples gallery
                const bool first_image_in_dir = p.parent_path() != last_example_parent_dir;
                auto source_file_2 = p.lexically_relative(root_path).string();
                if (first_image_in_dir) {
                    // First image is large
                    const auto large_figure_figure = final_figure_destination.lexically_proximate(root_path).string();
                    examples_gallery << "[![" << exe_file << "](" << large_figure_figure << ")](" << source_file_2 << ")\n\n";
                    last_example_parent_dir = p.parent_path();
                    // Next image is the second image in this gallery
                    second_image = true;
                } else {
                    if (second_image) {
                        examples_gallery << "More examples:\n\n";
                        second_image = false;
                    }
                    // Other images are thumbnails
                    examples_gallery << "[![" << exe_file << "](" << thumb_file << ")](" << source_file_2 << ")  ";
                }
            };

            bool dont_have_the_large_image_yet = !fs::exists(figure_destination_svg) && !fs::exists(figure_destination_png);
            if (dont_have_the_large_image_yet) {
                // Go to directory and run the executable
                string exe_str = executable.string();
                exe_str = regex_replace(exe_str, regex(" "), "\\ ");
                exe_str = regex_replace(exe_str, regex("~"), "\\~");

                string dir_str = p.parent_path().string();
                dir_str = regex_replace(dir_str, regex(" "), "\\ ");
                dir_str = regex_replace(dir_str, regex("~"), "\\~");

                string cmd = "cd ";
                cmd += dir_str;
                cmd += " && ";
                cmd += exe_str;
                cout << cmd << endl;
                int r = system(cmd.c_str());
                if (r != 0) {
                    std::cerr << "Could not execute " << cmd << std::endl;
                }

                // Move the figures to the documentation directory
                fs::path figure_origin_svg = p.parent_path() / "example.svg";
                if (fs::exists(figure_origin_svg)) {
                    if (!fs::exists(figure_destination_svg.parent_path())) {
                        fs::create_directories(
                            figure_destination_svg.parent_path());
                    }
                    fs::rename(figure_origin_svg, figure_destination_svg);
                } else {
                    cout << "Could not find " << figure_origin_svg.filename()
                         << " for " << executable.filename() << endl;
                }
            }

            // If we already have the svg image or were able to generate it above,
            // we create the png equivalent
            if (fs::exists(figure_destination_svg)) {
                // Convert the svg figure to png using librsvg
                // brew install librsvg
                // rsvg-convert -h 420 filename.svg > filename-420.png
                // We create the new png image even if it already exists
                // because we might be updating the png image from a new
                // svg we put there with this intention for some reason
                string cmd = "rsvg-convert \"" + figure_destination_svg.string() + "\" > \"" + figure_destination_png.string() + "\"";
                int r = system(cmd.c_str());
                if (r != 0) {
                    std::cerr << "Could not execute " << cmd << std::endl;
                }

                // Create a png thumbnail as well
                // We create the thumbnail even if it already exists
                // because we might be updating the thumbnail from a new
                // svg we put there with this intention for some reason
                // rsvg-convert -h 100 filename.svg > filename-100.png
                cmd = "rsvg-convert -h 100 \"" + figure_destination_svg.string() + "\" > \"" + figure_destination_thumbnail_png.string() + "\"";
                r = system(cmd.c_str());
                if (r != 0) {
                    std::cerr << "Could not execute " << cmd << std::endl;
                }

                // Keep only the smallest image (svg or png) for the large gallery
                // We need to do that because some svg examples would be huge
                //     for the purposes of documentation.
                // We give a small preference to svg images though.
                auto svg_size = fs::file_size(figure_destination_svg);
                auto png_size = fs::file_size(figure_destination_png);
                if (svg_size > 2 * png_size) {
                    fs::remove(figure_destination_svg);
                    final_figure_destination = figure_destination_png;
                } else {
                    fs::remove(figure_destination_png);
                    final_figure_destination = figure_destination_svg;
                }
            } else {
                // If the png image exists but the svg image does not
                // We assume we have run this executable already and
                // it has chosen the png over svg
                final_figure_destination = figure_destination_png;
            }

            // If the thumbnail exists, it means everything is OK
            // Unless we externally interfere with the examples,
            // we wouldn't have the thumbnail if we didn't have the
            // svg or png to generate it.
            const auto thumbnail_exists = fs::exists(figure_destination_thumbnail_png);
            if (thumbnail_exists) {
                // Update the galleries with the final images
                update_gallery();
            }
        } else {
            // If this path is not a cpp file we also update the gallery with headers
            // We include a header every time we are visiting a new directory
            // There will be no directory repetitions because of the queue
            const auto update_gallery = [&]() {
                // Delete gif file if any
                fs::path maybe_gif = p / "example.gif";
                if (fs::exists(maybe_gif)) {
                    std::cout << "Found duplicate gif! File " << maybe_gif.string() << std::endl;
                    fs::remove(maybe_gif);
                }

                // Create an appropriate header for this directory
                fs::path relative_path = p.lexically_relative(source_examples_path);

                // Create the header title string
                string header_title = relative_path.string();
                auto it = dir_map.find(header_title);
                if (it == dir_map.end()) {
                    std::cerr << "Could not find a header for " << header_title << std::endl;
                } else {
                    header_title = it->second;
                }

                // Create the header markdown hashes ("###...")
                size_t level = std::distance(relative_path.begin(),relative_path.end());
                std::string header_hashes(level + 1,'#' );

                // Append Header to the large image gallery
                if (p != last_example_dir) {
                    gallery << "\n" << header_hashes << " " << header_title << "\n\n";
                }

                // Nothing to append to the thumbnail gallery
                // Append header to the examples gallery
                if (p != last_example_dir) {
                    // Examples have an extra hash because they are meant to be pasted on
                    // the main README.md file later
                    examples_gallery << "\n" << header_hashes << "# " << header_title << "\n\n";
                }

                // Update the last directory for which we created a header
                if (p != last_example_dir) {
                    last_example_dir = p;
                }
            };
            update_gallery();
        }
    }

    /// Remove temporary directories if any
    if (fs::exists(root_path / "img")) {
        fs::rename(root_path / "img", documentation_path / "img");
    }

    if (fs::exists(root_path / "formats")) {
        fs::rename(root_path / "formats", documentation_path / "formats");
    }

    return 0;

}
