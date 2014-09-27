#pragma once

#include <Eigen/Eigen>

#include "Ray.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class RaycastResult
    {
    protected:
      bool objectHit;
      Ray nextCastRay;
      Eigen::Vector3f color;
    public:
      bool ObjectHit() const { return objectHit; }
      
      const Ray &NextCastRay() const { return nextCastRay; }
      Ray &NextCastRay() { return nextCastRay; }

      const Eigen::Vector3f &Color() const { return color; }
      Eigen::Vector3f &Color() { return color; }
    public:
      RaycastResult();
      RaycastResult(const Ray &nextCastRay, const Eigen::Vector3f &color);
    public:
      static const RaycastResult NoHit();


      double intensity = 0.0;
    };
  }
}