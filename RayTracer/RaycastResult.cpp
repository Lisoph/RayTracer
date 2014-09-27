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
    
    const RaycastResult &RaycastResult::NoHit()
    {
      static RaycastResult res;
      return res;
    }
  }
}