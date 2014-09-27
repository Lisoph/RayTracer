#pragma once

#include <Eigen/Eigen>

#include "SceneEntity.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class Light: public SceneEntity
    {
    protected:
      Eigen::Vector3f color;
    public:
      Light(const Eigen::Vector3d &pos);
      Light(const Eigen::Vector3d &pos, const Eigen::Vector3f &color);
      Light(const Light &other);
    public:
      const Eigen::Vector3f &Color() const { return color; }
      Eigen::Vector3f &Color() { return color; }
    public:
      Eigen::Vector3d GetSurfaceNormalAt(const Eigen::Vector3d &point) override;
      bool IntersectsWithRay(const Ray &ray) override;
    };
  }
}