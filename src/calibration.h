#ifndef PHOTOBUNDLE2_CALIBRATION_H
#define PHOTOBUNDLE2_CALIBRATION_H

#include "types.h"
#include "eigen.h"
#include <iostream>

#include <Eigen/LU>

/**
 * Stereo calibration using a pinhole model
 */
class Calibration
{
 public:
  inline Calibration() {}

  /**
   * \param K the intrinsics matrix
   * \param b the stereo baseline
   */
  inline Calibration(const Mat33& K, double b)
      : _K(K), _K_inv(_K.inverse()), _baseline(b) {}

  inline const double& b() const { return _baseline; }
  inline const double& fx() const { return _K(0,0); }
  inline const double& fy() const { return _K(1,1); }
  inline const double& cx() const { return _K(0,2); }
  inline const double& cy() const { return _K(1,2); }

  inline const Mat33& K() const { return _K; }
  inline const Mat33& Kinv() const { return _K_inv; }

  inline Mat33& K() { return _K; }
  inline double& baseline() { return _baseline; }

  template <typename T> inline
  void project(const T* X, T& u, T& v) const
  {
    u = (( X[0] * T(fx()) ) / X[2]) + T(cx());
    v = (( X[1] * T(fy()) ) / X[2]) + T(cy());
  }

  template <typename T> inline
  void project(const T* X, T* uv) const { return project(X, uv[0], uv[1]); }

  inline Vec2 project(const Vec3& X) const { return normHomog(_K * X); }

  template <typename PointType> inline
  Vec_<double,3> triangulate(const PointType& uvd) const
  {
    const auto u = uvd[0], v = uvd[1], d = uvd[2];
    double z = (b()*fx()) * (1.0 / d),
           x = (u - cx())*z/fx(),
           y = (v - cy())*z/fy();
    return Vec_<double,3>(x, y, z);
  }

  inline void setParameters(const double* p)
  {
    auto fx = p[0], fy = p[1], cx = p[2], cy = p[3];
    _K <<
        fx, 0.0, cx,
        0.0, fy, cy,
        0.0, 0.0, 1.0;
  }

 private:
  Mat33 _K, _K_inv;
  double _baseline;
}; // Calibration

#endif // PHOTOBUNDLE2_CALIBRATION_H
