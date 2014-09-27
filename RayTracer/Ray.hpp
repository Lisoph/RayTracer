#pragma once

#include <Eigen/Eigen>

namespace RayTracer
{
  namespace Tracing
  {
    class Ray
    {
    protected:
      Eigen::Vector3d origin;
      Eigen::Vector3d dir;
      double length;
    public:
      const Eigen::Vector3d &Origin() const { return origin; }
      Eigen::Vector3d &Origin() { return origin; }
      
      const Eigen::Vector3d &Dir() const { return dir; }
      Eigen::Vector3d &Dir() { return dir; }
      
      double Length() const { return length; }
      double &Length() { return length; }
    public:
      Ray();
      Ray(double length);

      Ray &operator = (const Ray &rhs);
    };
  }
}