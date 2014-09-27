#include "Material.hpp"

#include "Math.hpp"
#include "SceneEntity.hpp"

#include <cmath>

namespace RayTracer
{
  namespace Tracing
  {
    Material::Material()
    : color(1.0f, 1.0f, 1.0f)
    {}
    
    Material::Material(const Eigen::Vector3f &color)
    : color(color)
    {}
    
    Material::Material(const Material &copy)
    : color(copy.color)
    {}
    
    Material::~Material() {}
    
    /*Eigen::Vector3d Material::GetReflection(const Eigen::Vector3d &relativeRayHitPos,
                                  const Eigen::Vector3d &rayDir,
                                  const Eigen::Vector3d &normal)
    {
      return Math::Reflect(rayDir, normal);
    }*/

    Eigen::Vector3d Material::GetReflection(const Eigen::Vector3d &hitPos, const Eigen::Vector3d &rayDir, SceneEntity &hitEntity)
    {
      return Math::Reflect(rayDir, hitEntity.GetSurfaceNormalAt(hitPos));
    }
    
    Eigen::Vector3f Material::GetColorAt(const Eigen::Vector3d &relativePos)
    {
      return color;
    }
  }
}