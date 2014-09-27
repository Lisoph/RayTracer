#include "Math.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    namespace Math
    {
      Eigen::Vector3d Reflect(const Eigen::Vector3d &vec, const Eigen::Vector3d &normal)
      {
        return vec - ((normal * vec.normalized().dot(normal)) * 2);
        /*return vec - 2 * vec.dot(normal) * normal;*/
      }
    }
  }
}