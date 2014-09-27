#pragma once

#include <Eigen/Eigen>

namespace RayTracer
{
  namespace Tracing
  {
    namespace Math
    {
      Eigen::Vector3d Reflect(const Eigen::Vector3d &vec, const Eigen::Vector3d &normal);
    }
  }
}