#include <Eigen/Dense>

class SQualityMetric {
public:
  SQualityMetric();
  virtual double operator()(const Eigen::Matrix2d &Q,
                            const Eigen::Matrix2d &P) const;
  virtual double Evaluate(const Eigen::Matrix2d &Q,
                          const Eigen::Matrix2d &P) const;
  virtual std::pair<double, Eigen::Vector2d>
  EvaluateWithGrad(const Eigen::Matrix2d &Q, const Eigen::Matrix2d &P) const;
  virtual std::tuple<double, Eigen::Vector2d, Eigen::>
  EvaluateWithGrad(const Eigen::Matrix2d &Q, const Eigen::Matrix2d &P) const;
};

struct JQMetric {
  double operator()(const Eigen::Matrix3d &J, const Eigen::Matrix3d &P) const {
    auto K = J * P.inverse();
    return 3 * pow(determinant(K), 2 / 3) / trace(K.transpose() * K);
  }
