#include <matplot/matplot.h>

#include <cmath>
#include <random>

int main() {
  using namespace matplot;

  std::vector<double> votes = {35, 50, 30, 5, 80};
  std::vector<std::string> pies = {"Chocolate", "Apple", "Pecan", "Cherry",
                                   "Pumpkin"};

  pareto(votes, pies, 1.0);
  ylabel("Votes");

  wait();
  return 0;
}