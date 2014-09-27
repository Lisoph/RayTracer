#include "Ray.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    Ray::Ray()
    : origin(), dir(), length(0)
    {}
    
    Ray::Ray(double length)
    : origin(), dir(), length(length)
    {}

    Ray &Ray::operator = (const Ray &rhs)
    {
      origin = rhs.origin;
      dir = rhs.dir;
      length = rhs.length;
      return *this;
    }
  }
}