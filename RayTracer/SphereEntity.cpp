#include "SphereEntity.hpp"

#include <iostream>

namespace RayTracer
{
  namespace Tracing
  {
    SphereEntity::SphereEntity(const Eigen::Vector3d &pos, double radius, Material &mat)
      : SceneEntity(pos, Eigen::Vector3d(), mat), radius(radius)
    {}

    Eigen::Vector3d SphereEntity::GetSurfaceNormalAt(const Eigen::Vector3d &point)
    {
      return (point - pos).normalized();
    }

    bool SphereEntity::IntersectsWithRay(const Ray &ray)
    {
      /* Things are gonna get mathemagical now... */

#if 1
      Eigen::Vector3d center(pos); // Not sure if correct

      double t0, t1;
      Eigen::Vector3d L = center - ray.Origin();
      double tca = L.dot(ray.Dir());
      if(tca < 0) return false;
      double d2 = L.dot(L) - tca * tca;
      if(d2 > radius * 2) return false;
      double thc = sqrt(radius * 2 - d2);
      t0 = tca - thc;
      t1 = tca + thc;

      /* Let my try to calculate P... */
      Eigen::Vector3d P = ray.Origin() + t0 * ray.Dir();
      intersectionPoint = P;

      //if(t0 > ray.Length()) return false;
      //else ray.tmax = t0;
      return true;
#endif

#if 0
      /* I wrote that. Looks like it is working correctly, but I'm not 100% sure... */
      /* It doesn't. */
      double raySphereDist = (pos - ray.Origin()).norm();
      Eigen::Vector3d sphereOnRay = ray.Origin() + (ray.Dir() * raySphereDist);

      if((sphereOnRay - pos).norm() * 2 <= radius)
      {
        Eigen::Vector3d sphereOnRayToSphereDir = (pos - sphereOnRay).normalized();
        Eigen::Vector3d outerPoint = pos + (-sphereOnRayToSphereDir * radius);
        double sphereOnRayOuterPointDist = (sphereOnRay - outerPoint).norm();
        intersectionPoint = ray.Origin() + (ray.Dir() * (raySphereDist - sphereOnRayOuterPointDist));

        return true;
      }

      return false;
#endif

#if 0

#define sq(x) ((x)*(x))

      Eigen::Vector3d rayEnd = ray.Origin() + ray.Dir() * 1;

      double cx = pos(0);
      double cy = pos(1);
      double cz = pos(2);

      double x0 = ray.Origin()(0); // Maybe
      double y0 = ray.Origin()(1); // Maybe
      double z0 = ray.Origin()(2); // Maybe

      double x1 = rayEnd(0);
      double y1 = rayEnd(1);
      double z1 = rayEnd(2);

      double dx = /*std::abs*/(x1 - x0);
      double dy = /*std::abs*/(y1 - y0);
      double dz = /*std::abs*/(z1 - z0);

      double a = sq(dx) + sq(dy) + sq(dz);
      double b = 2 * dx * (x0 - cx) + 2 * dy * (y0 - cy) + 2 * dz * (z0 - cz);
      double c = sq(cx) + sq(cy) + sq(cz) + sq(x0) + sq(y0) + sq(z0) + -2 * (cx * x0 + cy * y0 + cz * z0) - sq(radius);

#define discri(a, b, c) (sq(b) - 4 * (a) * (c))

      double _discri = discri(a, b, c);

      if(_discri >= 0)
      {
        double t = (-b - std::sqrt(_discri)) / (2 * a);
        intersectionPoint(0) = x0 + t * dx;
        intersectionPoint(1) = y0 + t * dy;
        intersectionPoint(2) = z0 + t * dz;

        return true;
      }

      return false;

#undef discri
#undef sq

#endif
    }
  }
}