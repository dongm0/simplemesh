#ifndef ELEMENTQCALCULATOR_H
#define ELEMENTQCALCULATOR_H
#include <vector>

class ElementQCalculator {
  virtual double operator()(const std::vector<double> &vals) const;
};

struct ElementQMean {
  double operator()(const std::vector<double> &vals) const {
    double res = 0;
    int size = vals.size();
    for (auto x : vals) {
      res += x;
    }
    return res / size;
  }
};
#endif