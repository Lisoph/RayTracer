#include "Scene.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

namespace RayTracer
{
  namespace Tracing
  {
    Scene::Scene()
    : entities()
    {}
    
    unsigned int Scene::AddEntity(SceneEntity &entity)
    {
      unsigned int index = (unsigned int)entities.size();
      entities[index] = &entity;
      return index;
    }

    unsigned int Scene::AddLight(Light &light)
    {
      unsigned int index = (unsigned int)lights.size();
      lights[index] = &light;
      return index;
    }
    
    bool Scene::ExistsEntity(unsigned int entity) const
    {
      return (entities.find(entity) == entities.end() ? false : true);
    }

    bool Scene::ExistsLight(unsigned int light) const
    {
      return (lights.find(light) == lights.end() ? false : true);
    }
    
    bool Scene::RemoveEntity(unsigned int entity)
    {
      auto iter = entities.find(entity);
      if(iter == entities.end()) return false;
      
      entities.erase(iter);
      return true;
    }

    bool Scene::RemoveLight(unsigned int light)
    {
      auto iter = lights.find(light);
      if(iter == lights.end()) return false;

      lights.erase(iter);
      return true;
    }
    
    RaycastResult Scene::CastRay(const Ray &ray, const RaycastResult &previousCastResult)
    {
      std::vector<std::pair<double, SceneEntity*>> hitEntities;
      SceneEntity *hitEntity = nullptr;

      for(auto &pair : entities)
      {
        SceneEntity *ent = pair.second;

        if(ent->IntersectsWithRay(ray))
        {
          Eigen::Vector3d inters = ent->IntersectionPoint();
          double depth = (inters - ray.Origin()).norm();

          hitEntities.push_back(std::pair<double, SceneEntity*>(depth, ent));
        }
      }

      if(hitEntities.size() > 0)
      {
        /* Get nearest entity */
        std::sort(hitEntities.begin(), hitEntities.end(), [](const std::pair<double, SceneEntity*> &lhs, const std::pair<double, SceneEntity*> &rhs)
        {
          return lhs.first < rhs.first;
        });

        hitEntity = hitEntities[0].second;
      }

      if(hitEntity == nullptr)
        return RaycastResult::NoHit();

      /* So the cast ray hit something. Now we would need to check, if the hit point is visible for the lights. */
      /*
            for each light ->
              is hitPoint visible from light?
                yes -> add calculated light color to result color (don't forget to average)
                no -> result color = black
      
      */

      Eigen::Vector3d intersPoint = hitEntity->IntersectionPoint();
      int numAffectingLights = 0;
      Eigen::Vector3f resultingLightingColor = Eigen::Vector3f::Zero();
      double totalContri = 0;

      for(auto &pair : lights)
      {
        const Light *light = pair.second;

        if(IsLightVisibleFrom(*light, intersPoint))
        {
          Eigen::Vector3d surfNormal = hitEntity->GetSurfaceNormalAt(intersPoint);
          Eigen::Vector3d pointToLightDir = (light->Pos() - intersPoint).normalized();
          double contri = std::max(surfNormal.dot(pointToLightDir), 0.0);

          resultingLightingColor += light->Color() * (float)contri;
          totalContri += contri;
          ++numAffectingLights;
        }
      }

      if(numAffectingLights > 0)
      {
        resultingLightingColor /= (float)numAffectingLights;
        totalContri /= numAffectingLights;
      }

      Ray newRay(ray.Length());
      newRay.Origin() = hitEntity->IntersectionPoint();
      newRay.Dir() = hitEntity->Mat().GetReflection(hitEntity->IntersectionPoint(), ray.Dir(), *hitEntity);

      /*Eigen::Vector3f resultColor = hitEntity->Mat().GetColorAt(hitEntity->IntersectionPoint());
      resultColor += resultingLightingColor;*/

      Eigen::Vector3f resultColor = Eigen::Vector3f::Zero();
      Eigen::Vector3f matColor = hitEntity->Mat().GetColorAt(hitEntity->IntersectionPoint());
      if(numAffectingLights > 0)
      {
        //resultColor = matColor * resultingLightingColor; // WTF Eigen
        /*resultColor(0) = matColor(0) * resultingLightingColor(0);
        resultColor(1) = matColor(1) * resultingLightingColor(1);
        resultColor(2) = matColor(2) * resultingLightingColor(2);*/

        resultColor = matColor * 0.4f + resultingLightingColor * 0.6f;
        resultColor /= 2.0f;
      }
      else
      {
        resultColor = matColor * 0.04f + resultingLightingColor * 0.06f;
        //resultColor /= 2.0f;
      }

      auto rcres = RaycastResult(newRay, resultColor);
      rcres.intensity = totalContri;
      return rcres;
    }

    
    bool Scene::IsLightVisibleFrom(const Light &light, const Eigen::Vector3d &point) /* Probably broken */
    {
      Eigen::Vector3d pointToLight = light.Pos() - point;
      double pointToLightDist = pointToLight.norm();
      Eigen::Vector3d pointToLightDir = pointToLight.normalized();
      
      Ray ray(pointToLightDist);
      ray.Origin() = point;
      ray.Dir() = pointToLightDir;

      bool hitEntity = false;

      for(auto &pair : entities)
      {
        SceneEntity *ent = pair.second;
        if(ent->IntersectsWithRay(ray))
        {
          hitEntity = true;
          break;
        }
      }

      /*static bool once = false;
      if(!once && !hitEntity)
      {
        std::cout << "Light pos:\n" << light.Pos() << '\n';
        std::cout << "pointToLightDir:\n" << pointToLightDir << '\n';
        std::cout << "point:\n" << point << '\n';
        once = true;
      }*/

      return !hitEntity;
    }
  }
}