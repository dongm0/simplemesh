#include <vector>

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