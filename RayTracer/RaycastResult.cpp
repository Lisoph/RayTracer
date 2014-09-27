#include "RaycastResult.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    RaycastResult::RaycastResult()
      : objectHit(false), nextCastRay(), color()
    {}
    
    RaycastResult::RaycastResult(const Ray &nextCastRay, const Eigen::Vector3f &color)
      : objectHit(true), nextCastRay(nextCastRay), color(color)
    {}
    
    const RaycastResult RaycastResult::NoHit()
    {
      RaycastResult res;
      res.Color() = Eigen::Vector3f(0.2f, 0.4f, 0.8f);
      return res;
    }
  }
}