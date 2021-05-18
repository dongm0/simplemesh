#ifndef JQMETRIC_H
#define JQMETRIC_H
#include <Eigen/Dense>

class SQualityMetric {
public:
  SQualityMetric();
  virtual double operator()(const Eigen::Matrix2d &Q,
                            const Eigen::Matrix2d &P) const;
  virtual double Evaluate(const Eigen::Matrix2d &Q,
                          const Eigen::Matrix2d &P) const;
  virtual std::pair<double, Eigen::Vector4d>
  EvaluateWithGrad(const Eigen::Matrix2d &Q, const Eigen::Matrix2d &P) const;
  virtual std::tuple<double, Eigen::Vector4d, Eigen::Matrix4d>
  EvaluateWithHessian(const Eigen::Matrix2d &Q, const Eigen::Matrix2d &P) const;
  virtual double Normalize(double raw_val) const;
};
#endif