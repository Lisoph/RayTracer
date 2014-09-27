#include "Light.hpp"

#include <stdexcept>

namespace RayTracer
{
  namespace Tracing
  {
    Light::Light(const Eigen::Vector3d &pos)
      : SceneEntity(pos), color()
    {}
    
    Light::Light(const Eigen::Vector3d &pos, const Eigen::Vector3f &color)
      : SceneEntity(pos), color(color)
    {}

    Light::Light(const Light &other)
      : Light(other.Pos(), other.Color())
    {}

    Eigen::Vector3d Light::GetSurfaceNormalAt(const Eigen::Vector3d &point)
    {
      throw std::runtime_error("Called GetSurfaceNormalAt on a light");
    }

    bool Light::IntersectsWithRay(const Ray &ray)
    {
      throw std::runtime_error("Called IntersectsWithRay on a light");
    }
  }
}