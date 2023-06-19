#include <matplot/matplot.h>
#include <random>

using namespace matplot;
using namespace std;

class americas_trip_solver {
  public:
    americas_trip_solver(const vector<double> &lat, const vector<double> &lon,
                         const vector<string> &names, axes_handle ax);
    void run(size_t iterations = 100);

  private:
    void setup_axes();
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
    line_handle lh_;

    // Current tour
    double curr_dist_{0.0};
    vector<size_t> curr_tour_;

    // Best tour
    double min_dist_{0.0};
    vector<size_t> best_tour_;
};

americas_trip_solver::americas_trip_solver(const vector<double> &lat,
                                           const vector<double> &lon,
                                           const vector<string> &names,
                                           axes_handle ax)
    : lat_(lat), lon_(lon), names_(names), ax_(ax) {
    setup_axes();
}

void americas_trip_solver::run(size_t iterations) {
    for (size_t i = 0; i < iterations; ++i) {
        setup_starting_point(i);
        iteration();
    }
    ax_->draw();
    ax_->parent()->save("americastrip.svg");
}

void americas_trip_solver::setup_starting_point(size_t iteration) {
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
americas_trip_solver::get_neighbors(const std::vector<size_t> &tour) {
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

void americas_trip_solver::iteration() {
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

double americas_trip_solver::tour_distance(const vector<size_t> &tour) {
    double sum = 0.;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        sum += distance(lon_[tour[i]], lat_[tour[i]], lon_[tour[i + 1]],
                        lat_[tour[i + 1]]);
    }
    sum += distance(lon_[tour[tour.size() - 1]], lat_[tour[tour.size() - 1]],
                    lon_[tour[0]], lat_[tour[0]]);
    return sum;
}

void americas_trip_solver::draw() {
    vector<double> sorted_lat;
    vector<double> sorted_lon;
    for (const size_t &idx : best_tour_) {
        sorted_lat.emplace_back(lat_[idx]);
        sorted_lon.emplace_back(lon_[idx]);
    }
    sorted_lat.emplace_back(lat_[best_tour_[0]]);
    sorted_lon.emplace_back(lon_[best_tour_[0]]);
    lh_->x_data(sorted_lon);
    lh_->y_data(sorted_lat);
    ax_->title("Tour distance " + num2str(min_dist_));
    ax_->draw();
}

bool americas_trip_solver::update_if_better(const vector<size_t> &neighbor) {
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

void americas_trip_solver::draw_if_improvement() {
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

void americas_trip_solver::setup_axes() {
    ax_->clear();
    ax_->geolimits(min(lat_) - 5, max(lat_) + 5, min(lon_) - 30,
                   max(lon_) + 50);
    lh_ = ax_->geoplot(lat_, lon_);
    ax_->hold(true);
    ax_->geoscatter(lat_, lon_);
    auto [lon_c, lat_c, names_c] =
        clear_overlapping_labels(lon_, lat_, names_, 2, 2);
    ax_->text(lon_c, lat_c, names_c);
    ax_->draw();
}

int main() {
    vector<string> names = {
        "Marigot",        "The Valley",     "Saint John's",    "Buenos Aires",
        "Oranjestad",     "Nassau",         "Bridgetown",      "Belmopan",
        "Hamilton",       "La Paz",         "Brasilia",        "Road Town",
        "Ottawa",         "George Town",    "Santiago",        "Bogota",
        "San Jose",       "Havana",         "Willemstad",      "Roseau",
        "Santo Domingo",  "Quito",          "San Salvador",    "Stanley",
        "Nuuk",           "Saint George's", "Guatemala City",  "Georgetown",
        "Port-au-Prince", "Tegucigalpa",    "Kingston",        "Mexico City",
        "Plymouth",       "Managua",        "Panama City",     "Asuncion",
        "Lima",           "San Juan",       "Gustavia",        "Basseterre",
        "Castries",       "Saint-Pierre",   "Kingstown",       "Philipsburg",
        "Paramaribo",     "Port of Spain",  "Grand Turk",      "Washington",
        "Montevideo",     "Caracas",        "Charlotte Amalie"};

    vector<double> lat = {
        18.0731,  18.2166, 17.1166, -34.5833, 12.5166, 25.0833, 13.1,
        17.25,    32.2833, -16.5,   -15.7833, 18.4166, 45.4166, 19.3,
        -33.45,   4.6,     9.9333,  23.1166,  12.1,    15.3,    18.4666,
        -0.2166,  13.7,    -51.7,   64.1833,  12.05,   14.6166, 6.8,
        18.5333,  14.1,    18,      19.4333,  16.7,    12.1333, 8.9666,
        -25.2666, -12.05,  18.4666, 17.8833,  17.3,    14,      46.7666,
        13.1333,  18.0166, 5.8333,  10.65,    21.4666, 38.8833, -34.85,
        10.4833,  18.35};

    vector<double> lon = {
        -63.0822, -63.0500, -61.8500, -58.6666, -70.0333, -77.3500, -59.6166,
        -88.7666, -64.7833, -68.1500, -47.9166, -64.6166, -75.7000, -81.3833,
        -70.6666, -74.0833, -84.0833, -82.3500, -68.9166, -61.4000, -69.9000,
        -78.5000, -89.2000, -57.8500, -51.7500, -61.7500, -90.5166, -58.1500,
        -72.3333, -87.2166, -76.8000, -99.1333, -62.2166, -86.2500, -79.5333,
        -57.6666, -77.0500, -66.1166, -62.8500, -62.7166, -61.0000, -56.1833,
        -61.2166, -63.0333, -55.1666, -61.5166, -71.1333, -77.0000, -56.1666,
        -66.8666, -64.9333};

    figure_handle f = figure(true);
    americas_trip_solver s(lat, lon, names, f->current_axes());
    s.run();

    show();
    return 0;
}