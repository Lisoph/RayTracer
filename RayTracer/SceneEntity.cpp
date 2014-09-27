#include "SceneEntity.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    SceneEntity::SceneEntity()
      : pos(), rot(), mat(Material::Instance()), intersectionPoint()
    {}
    
    SceneEntity::SceneEntity(const SceneEntity &other)
      : pos(other.pos), rot(other.rot), mat(other.mat), intersectionPoint()
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos)
      : pos(pos), rot(), mat(Material::Instance())
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot)
      : pos(pos), rot(rot), mat(Material::Instance()), intersectionPoint()
    {}
    
    SceneEntity::SceneEntity(Material &mat)
      : pos(), rot(), mat(mat), intersectionPoint()
    {}
    
    SceneEntity::SceneEntity(const Eigen::Vector3d &pos, const Eigen::Vector3d &rot,
                             Material &mat)
      : pos(pos), rot(rot), mat(mat), intersectionPoint()
    {}
    
    SceneEntity::~SceneEntity() {}
    
    SceneEntity &SceneEntity::operator = (const SceneEntity &rhs)
    {
      pos = rhs.pos;
      rot = rhs.rot;
      mat = rhs.mat;
      
      return *this;
    }
  }
}