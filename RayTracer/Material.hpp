#pragma once

#include <Eigen/Eigen>

namespace RayTracer
{
  namespace Tracing
  {
    class SceneEntity;

    class Material
    {
    protected:
      /* The base color of an object */
      Eigen::Vector3f color;
    public:
      Material();
      Material(const Eigen::Vector3f &color);
      Material(const Material &copy);
      virtual ~Material();
      
      static Material &Instance()
      {
        static Material inst;
        return inst;
      }
    public:
      const Eigen::Vector3f &Color() const { return color; }
      Eigen::Vector3f &Color() { return color; }
    public:
      /* relativeRayHitPos should be: absolute hit pos minus hit object position.
         rayDir should be the direction vector of the last object the ray hit (or cam), to the current object */
      /*virtual Eigen::Vector3d GetReflection(const Eigen::Vector3d &relativeRayHitPos,
                                            const Eigen::Vector3d &rayDir,
                                            const Eigen::Vector3d &normal);*/
      /* TODO: Rethink the function arguments above! */
      /* Passing the absolute hit pos, rayDir and a reference to the SceneEntity
         would make more sense.
       */


      virtual Eigen::Vector3d GetReflection(const Eigen::Vector3d &hitPos, const Eigen::Vector3d &rayDir, SceneEntity &hitEntity);
      
      virtual Eigen::Vector3f GetColorAt(const Eigen::Vector3d &relativePos);
    };
  }
}