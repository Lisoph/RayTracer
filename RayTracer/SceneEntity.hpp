#pragma once

#include <memory>

#include <Eigen/Eigen>

#include "Material.hpp"
#include "Ray.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class SceneEntity
    {
    public:
      typedef std::unique_ptr<Material> MaterialRef;
    protected:
      Eigen::Vector3d pos, rot;
      MaterialRef mat;
      Eigen::Vector3d intersectionPoint;
      bool lightingImmune;
    public:
      const Eigen::Vector3d &Pos() const { return pos; }
      Eigen::Vector3d &Pos() { return pos; }
      
      const Eigen::Vector3d &Rot() const { return rot; }
      Eigen::Vector3d &Rot() { return rot; }
      
      const MaterialRef &Mat() const { return mat; }
      MaterialRef &Mat() { return mat; }

      const Eigen::Vector3d &IntersectionPoint() const { return intersectionPoint; }
      
      bool LightingImmune() const { return lightingImmune; }
      void LightingImmune(bool l) { lightingImmune = l; }
    public:
      SceneEntity();
      SceneEntity(const SceneEntity &other);
      SceneEntity(const Eigen::Vector3d &pos);
      SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot);
      SceneEntity(MaterialRef &&mat);
      SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot,
                  MaterialRef &&mat);
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