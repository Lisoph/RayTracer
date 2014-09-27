#include "SceneEntity.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    /* What a mess */
    
    SceneEntity::SceneEntity()
      : pos(), rot(), mat(), intersectionPoint(), lightingImmune(false)
    {}
    
    SceneEntity::SceneEntity(const SceneEntity &other)
    : pos(other.pos), rot(other.rot), mat(new Material(*other.mat)), intersectionPoint(), lightingImmune(false)
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos)
      : pos(pos), rot(), mat(), lightingImmune(false)
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot)
      : pos(pos), rot(rot), mat(), intersectionPoint(), lightingImmune(false)
    {}
    
    SceneEntity::SceneEntity(MaterialRef &&mat)
    : pos(), rot(), mat(std::move(mat)), intersectionPoint(), lightingImmune(false)
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot,
                             MaterialRef &&mat)
    : pos(pos), rot(rot), mat(std::move(mat)), intersectionPoint(), lightingImmune(false)
    {}
    
    SceneEntity::~SceneEntity() {}
    
    SceneEntity &SceneEntity::operator = (const SceneEntity &rhs)
    {
      pos = rhs.pos;
      rot = rhs.rot;
      mat.reset(new Material(*rhs.mat));
      
      return *this;
    }
  }
}