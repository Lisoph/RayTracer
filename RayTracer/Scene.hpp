#pragma once

#include <map>
#include <functional>

#include "SceneEntity.hpp"
#include "RaycastResult.hpp"
#include "Light.hpp"

namespace RayTracer
{
  namespace Tracing
  {
    class Scene
    {
    public:
      typedef std::map<unsigned int, SceneEntity*> EntityContainer;
      typedef std::map<unsigned int, Light*> LightContainer;
    protected:
      EntityContainer entities;
      LightContainer lights;
    public:
      Scene();
    public:
      unsigned int AddEntity(SceneEntity &entity);
      unsigned int AddLight(Light &light);

      bool ExistsEntity(unsigned int entity) const;
      bool ExistsLight(unsigned int light) const;

      bool RemoveEntity(unsigned int entity);
      bool RemoveLight(unsigned int light);
      
      size_t NumEntites() const { return entities.size(); }
      size_t NumLights() const { return lights.size(); }
      
      RaycastResult CastRay(const Ray &ray, const RaycastResult &previousCastResult);
    protected:
      bool IsLightVisibleFrom(const Light &light, const Eigen::Vector3d &point);
    };
  }
}