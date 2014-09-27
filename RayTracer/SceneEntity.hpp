#pragma once

#include <Eigen/Eigen>

#include "Material.hpp"
#include "Ray.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class SceneEntity
    {
    protected:
      Eigen::Vector3d pos, rot;
      Material &mat;
      Eigen::Vector3d intersectionPoint;
    public:
      const Eigen::Vector3d &Pos() const { return pos; }
      Eigen::Vector3d &Pos() { return pos; }
      
      const Eigen::Vector3d &Rot() const { return rot; }
      Eigen::Vector3d &Rot() { return rot; }
      
      const Material &Mat() const { return mat; }
      Material &Mat() { return mat; }

      const Eigen::Vector3d &IntersectionPoint() const { return intersectionPoint; }
    public:
      SceneEntity();
      SceneEntity(const SceneEntity &other);
      SceneEntity(const Eigen::Vector3d &pos);
      SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot);
      SceneEntity(Material &mat);
      SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot,
                  Material &mat);
      virtual ~SceneEntity();
      
      SceneEntity &operator = (const SceneEntity &rhs);
    public:
      /*
        It is assumed, that point is in absolute coordinates.
      */
      virtual Eigen::Vector3d GetSurfaceNormalAt(const Eigen::Vector3d &point) = 0;

      virtual bool IntersectsWithRay(const Ray &ray) = 0;
    };
  }
}