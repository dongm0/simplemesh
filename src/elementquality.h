#include "elementqmean.h"
#include "jqmetric.h"
#include "simplemesh.h"

std::vector<double> ElementQuality(const SimpleMesh &mesh, JQMetric m,
                                   ElementQMean qm) {
  std::vector<double> res;
  for (int i = 0; i < mesh.c.size(); ++i) { // 用迭代器写
    std::vector<double> _q;
    for (auto x : mesh.CellCorners(i)) {
      Eigen::Matrix2d Q;
      Q.row(0) = mesh.GetVertexCoord(x[1]) - mesh.GetVertexCoord(x[0]);
      Q.row(1) = mesh.GetVertexCoord(x[2]) - mesh.GetVertexCoord(x[0]);
      Q.transposeinplace();
      Eigen::Matrix2d P;
      P << 1, 0, 0, 1;
      _q.push_back(m(Q, P));
    }
    res.push_back(qm(m));
  }
  return res;
}