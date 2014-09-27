#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include <Eigen/Eigen>

#include "PixelBuffer.hpp"
#include "PPMExporter.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "SphereEntity.hpp"

using namespace RayTracer;

const int Width = 1920 * 4;
const int Height = 1280 * 4;

bool printDebug = false;

Eigen::Vector3d CalculateRayStartDir(const Eigen::Vector2d &pixelPos, const Eigen::Vector2d &resolution, const Eigen::Vector3d &rayOrigin)
{
  const Eigen::Vector2d halfRes = resolution / 2;
  const double ratio = resolution(0) / resolution(1);
  
  Eigen::Vector3d pixelPos3d;
  pixelPos3d(0) = ((pixelPos(0) - halfRes(0)) / halfRes(0)) * ratio;
  pixelPos3d(1) = -(pixelPos(1) - halfRes(1)) / halfRes(1);
  pixelPos3d(2) = 1.0;

  return (pixelPos3d - rayOrigin).normalized();
}

template <typename T>
T RandReal(const T &min, const T &max)
{
  static std::default_random_engine randEngine((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<T> distr(min, max);
  return distr(randEngine);
}

double SpawnRandomSpheres(Tracing::Scene &scene, int numSpheres, const Eigen::Vector3d &posMin, const Eigen::Vector3d &posMax,
                          double radiusMin, double radiusMax)
{
  using SceneEntityRef = Tracing::Scene::SceneEntityRef;
  using MaterialRef = Tracing::SceneEntity::MaterialRef;
  
  double avgZ = 0;
  
  for(int i = 0; i < numSpheres; ++i)
  {
    Eigen::Vector3d spherePos(RandReal<double>(posMin(0), posMax(0)), RandReal<double>(posMin(1), posMax(1)),
                              RandReal<double>(posMin(2), posMax(2)));
    double sphereRadius = RandReal<double>(radiusMin, radiusMax);
    Eigen::Vector3f sphereColor(RandReal<float>(0, 1), RandReal<float>(0, 1), RandReal<float>(0, 1));
    
    if(i == numSpheres / 2) sphereRadius = 20;
    avgZ += spherePos(2);
    
    scene.AddEntity(new Tracing::SphereEntity(spherePos, sphereRadius, MaterialRef(new Tracing::Material(sphereColor))));
  }
  
  avgZ /= numSpheres;
  return avgZ;
}

void SpawnSpherePlane(Tracing::Scene &scene, const Eigen::Vector3d &pos, double radius, int width, int height)
{
  using MaterialRef = Tracing::SceneEntity::MaterialRef;
  
  const double halfWidth = (width * radius) / 2, halfHeight = (height * radius) / 2;
  const double halfRadius = radius / 2;
  
  for(int y = 0; y < height; ++y)
  {
    for(int x = 0; x < width; ++x)
    {
      double X = x * radius, Y = y * radius;
      
      Eigen::Vector3d spherePos = Eigen::Vector3d((X - halfWidth) + halfRadius, 0, (Y - halfHeight) + halfRadius) + pos;
      Eigen::Vector3f sphereColor(RandReal<float>(0, 1), RandReal<float>(0, 1), RandReal<float>(0, 1));
      
      scene.AddEntity(new Tracing::SphereEntity(spherePos, radius, MaterialRef(new Tracing::Material(sphereColor))));
    }
  }
}

int main(int argc, char **argv)
{
  PixelBuffer myImage(Width, Height);
  
  Tracing::Scene myScene;
  const int numIters = 150; /* Maximum QUALITY Kappa */
  
  std::cout << "Casting...\n";
  
  using MaterialRef = Tracing::SceneEntity::MaterialRef;
  
  /* Let's add random spheres */
  /*double avgZ = SpawnRandomSpheres(myScene, 10, Eigen::Vector3d(-30, -30, 30), Eigen::Vector3d(30, 30, 40),
                                   5, 20);*/
  
  
  SpawnSpherePlane(myScene, Eigen::Vector3d(0, -15, 30), 5, 5, 5);
  
  myScene.AddEntity(new Tracing::SphereEntity(Eigen::Vector3d(0, 0, 25), 10,
                                              MaterialRef(new Tracing::Material(Eigen::Vector3f(1, 0, 0)))));
  
  SpawnRandomSpheres(myScene, 3, Eigen::Vector3d(-30, -25, 20), Eigen::Vector3d(30, 25, 50), 3, 3);
  
  double avgZ = 30;

  /* Lights */
  Tracing::Light *myLight = new Tracing::Light(Eigen::Vector3d(0, 40, avgZ), Eigen::Vector3f(1.0f, 1.0f, 1.0f));
  myScene.AddLight(myLight);
  
  /* Main loop */
  for(int y = 0; y < myImage.Height(); ++y)
  {
    for(int x = 0; x < myImage.Width(); ++x)
    {
      Eigen::Vector3f pixelColor = Eigen::Vector3f::Zero();

      /* First iteration */
      Tracing::Ray ray(100.0);
      ray.Origin() = Eigen::Vector3d::Zero();
      ray.Dir() = CalculateRayStartDir(Eigen::Vector2d(x, y), Eigen::Vector2d(Width, Height), ray.Origin());
      
      Tracing::RaycastResult result = myScene.CastRay(ray, Tracing::RaycastResult::NoHit());
      pixelColor = result.Color();

      double intensity = result.intensity;
      
      /* We then continue to cast rays, until we hit nothing or the iteration threshold is reached. */
      int doneIters;
      for(doneIters = 0; doneIters < numIters && result.ObjectHit(); ++doneIters)
      {
        ray = result.NextCastRay();
        result = myScene.CastRay(ray, result);
        if(result.ObjectHit()) pixelColor += result.Color() * /*(float)intensity*/ 0.6;

        intensity *= result.intensity;
        intensity = std::max(intensity, 0.0);
      }
      
      /* Average pixel color */
      pixelColor /= (float)(doneIters + 1);

      myImage.SetPixel(x, y, pixelColor);
    }
  }
  
  std::cout << "Exporting...\n";
  if(!Exporter::ExportPPM("TestTracer.ppm", myImage))
  {
    std::cout << "Could not export to ppm!\n";
  }
  
  std::cout << "Done\n";
}

