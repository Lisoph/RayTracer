#pragma once

#include "SceneEntity.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class SphereEntity: public SceneEntity
    {
    protected:
      double radius;
    public:
      SphereEntity(const Eigen::Vector3d &pos, double radius, MaterialRef &&mat);
      SphereEntity(const SphereEntity &copy);
    public:
      Eigen::Vector3d GetSurfaceNormalAt(const Eigen::Vector3d &point) override;

      bool IntersectsWithRay(const Ray &ray) override;
    };
  }
}