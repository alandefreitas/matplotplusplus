#include <matplot/matplot.h>
#include <random>

using namespace matplot;
using namespace std;

class eurotrip_solver {
  public:
    eurotrip_solver(const vector<double> &lat, const vector<double> &lon,
                    const vector<string> &names, axes_handle ax);
    void run(size_t iterations = 100);

  private:
    void setup_starting_point(size_t iteration);
    double tour_distance(const vector<size_t> &tour);
    void iteration();
    static vector<vector<size_t>>
    get_neighbors(const std::vector<size_t> &tour);
    bool update_if_better(const vector<size_t> &neighbor);
    void draw_if_improvement();
    void draw();

  private:
    vector<double> lat_;
    vector<double> lon_;
    vector<string> names_;
    axes_handle ax_;

    // Current tour
    double curr_dist_{0.0};
    vector<size_t> curr_tour_;

    // Best tour
    double min_dist_{0.0};
    vector<size_t> best_tour_;
};

eurotrip_solver::eurotrip_solver(const vector<double> &lat,
                                 const vector<double> &lon,
                                 const vector<string> &names, axes_handle ax)
    : lat_(lat), lon_(lon), names_(names), ax_(ax) {}

void eurotrip_solver::run(size_t iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        setup_starting_point(i);
        iteration();
    }
    ax_->draw();
    ax_->parent()->save("eurotrip.svg");
}

void eurotrip_solver::setup_starting_point(size_t iteration) {
    std::cout << "Starting point " << iteration << std::endl;
    if (iteration < names_.size()) {
        auto [lon_ignore, lat_ignore, tour] =
            greedy_tsp_with_idx(lon_, lat_, iteration);
        (void) lon_ignore;
        (void) lat_ignore;
        curr_tour_ = tour;
        curr_dist_ = tour_distance(curr_tour_);
    } else {
        static std::mt19937 g((std::random_device())());
        std::iota(curr_tour_.begin(), curr_tour_.end(), 0);
        std::shuffle(curr_tour_.begin(), curr_tour_.end(), g);
        curr_dist_ = tour_distance(curr_tour_);
    }
    if (iteration == 0 || curr_dist_ < min_dist_) {
        best_tour_ = curr_tour_;
        min_dist_ = curr_dist_;
    }
}

vector<vector<size_t>>
eurotrip_solver::get_neighbors(const std::vector<size_t> &tour) {
    vector<vector<size_t>> neighbors;
    constexpr size_t n_movements = 2;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        for (size_t j = i + 1; j < tour.size(); ++j) {
            for (size_t movement = 0; movement < n_movements; ++movement) {
                vector<size_t> neighbor = tour;
                if (movement == 0) {
                    std::swap(neighbor[i], neighbor[j]);
                } else {
                    std::reverse(neighbor.begin() + i,
                                 neighbor.begin() + j + 1);
                }
                neighbors.emplace_back(neighbor);
            }
        }
    }
    return neighbors;
}

void eurotrip_solver::iteration() {
    bool improvement = true;
    while (improvement) {
        improvement = false;
        for (const auto &neighbor : get_neighbors(curr_tour_)) {
            improvement = update_if_better(neighbor);
            draw_if_improvement();
            if (improvement) {
                break;
            }
        }
    }
}

double eurotrip_solver::tour_distance(const vector<size_t> &tour) {
    double sum = 0.;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        sum += distance(lon_[tour[i]], lat_[tour[i]], lon_[tour[i + 1]],
                        lat_[tour[i + 1]]);
    }
    sum += distance(lon_[tour[tour.size() - 1]], lat_[tour[tour.size() - 1]],
                    lon_[tour[0]], lat_[tour[0]]);
    return sum;
}

void eurotrip_solver::draw() {
    ax_->clear();

    ax_->geolimits(min(lat_) - 5, max(lat_) + 5, min(lon_) - 2, max(lon_) + 10);

    vector<double> sorted_lat;
    vector<double> sorted_lon;
    for (const size_t &idx : best_tour_) {
        sorted_lat.emplace_back(lat_[idx]);
        sorted_lon.emplace_back(lon_[idx]);
    }
    sorted_lat.emplace_back(lat_[best_tour_[0]]);
    sorted_lon.emplace_back(lon_[best_tour_[0]]);
    ax_->geoplot(sorted_lat, sorted_lon);

    ax_->hold(true);
    ax_->geoscatter(lat_, lon_);

    auto [lon_c, lat_c, names_c] =
        clear_overlapping_labels(lon_, lat_, names_, 1, 1);
    ax_->text(lon_c, lat_c, names_c);

    ax_->title("Tour distance " + num2str(min_dist_));

    ax_->draw();
}

bool eurotrip_solver::update_if_better(const vector<size_t> &neighbor) {
    double d = tour_distance(neighbor);
    if (d < curr_dist_) {
        curr_tour_ = neighbor;
        curr_dist_ = d;
        if (d < min_dist_) {
            best_tour_ = curr_tour_;
            min_dist_ = d;
        }
        return true;
    }
    return false;
}

void eurotrip_solver::draw_if_improvement() {
    static auto last_draw =
        chrono::high_resolution_clock::now() - chrono::seconds(1);
    static auto min_dist_when_last_draw = min_dist_;
    const auto current_time = chrono::high_resolution_clock::now();
    const bool its_been_a_while = current_time - last_draw > chrono::seconds(1);
    const bool things_are_better = min_dist_ < min_dist_when_last_draw;
    if (its_been_a_while && things_are_better) {
        last_draw = current_time;
        min_dist_when_last_draw = min_dist_;
        draw();
    }
}

int main() {
    vector<string> names = {"Tirana",    "Andorra la Vella",
                            "Vienna",    "Minsk",
                            "Brussels",  "Sarajevo",
                            "Sofia",     "Zagreb",
                            "Prague",    "Copenhagen",
                            "Tallinn",   "Helsinki",
                            "Paris",     "Berlin",
                            "Athens",    "Budapest",
                            "Reykjavik", "Dublin",
                            "Rome",      "Pristina",
                            "Riga",      "Vaduz",
                            "Vilnius",   "Luxembourg",
                            "Valletta",  "Chisinau",
                            "Monaco",    "Podgorica",
                            "Amsterdam", "Skopje",
                            "Oslo",      "Warsaw",
                            "Lisbon",    "Bucharest",
                            "Moscow",    "San Marino",
                            "Belgrade",  "Bratislava",
                            "Ljubljana", "Madrid",
                            "Stockholm", "Bern",
                            "Kiev",      "London"};

    vector<double> lat = {
        +41.3317, +42.5075, +48.2092, +53.9678, +50.8371, +43.8608, +42.7105,
        +45.8150, +50.0878, +55.6763, +59.4389, +60.1699, +48.8567, +52.5235,
        +37.9792, +47.4984, +64.1353, +53.3441, +41.8955, +42.6740, +56.9465,
        +47.1411, +54.6896, +49.6100, +35.9042, +47.0167, +43.7325, +42.4602,
        +52.3738, +42.0024, +59.9138, +52.2297, +38.7072, +44.4479, +55.7558,
        +43.9424, +44.8048, +48.2116, +46.0514, +40.4167, +59.3328, +46.9480,
        +50.4422, +51.5002};

    vector<double> lon = {
        +19.8172, +1.5218,  +16.3728, +27.5766, +4.3676,  +18.4214, +23.3238,
        +15.9785, +14.4205, +12.5681, +24.7545, +24.9384, +2.3510,  +13.4115,
        +23.7166, +19.0408, -21.8952, -6.2675,  +12.4823, +21.1788, +24.1049,
        +9.5215,  +25.2799, +6.1296,  +14.5189, +28.8497, +7.4189,  +19.2595,
        +4.8910,  +21.4361, +10.7387, +21.0122, -9.1355,  +26.0979, +37.6176,
        +12.4578, +20.4781, +17.1547, +14.5060, -3.7033,  +18.0645, +7.4481,
        +30.5367, -0.1262};

    std::cout << names.size() << " cities" << std::endl;

    figure_handle f = figure(true);
    eurotrip_solver s(lat, lon, names, f->current_axes());
    s.run();

    show();
    return 0;
}