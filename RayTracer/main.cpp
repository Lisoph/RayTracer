#include <iostream>
#include <vector>

#include <Eigen/Eigen>

#include "PixelBuffer.hpp"
#include "PPMExporter.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "SphereEntity.hpp"

#include <omp.h>

using namespace RayTracer;

const int Width = 640 * 1;
const int Height = 480 * 1;
const double RayLength = 100.0;

bool printDebug = false;

#if 0
struct CastResult
{
  Eigen::Vector3f color;
  bool didHit;
  Eigen::Vector3d hitPos;
  Eigen::Vector3d newDir;
};
#endif

#if 0
CastResult DoIteration(Tracing::Scene &scene, const Eigen::Vector3d &rayOrigin, const Eigen::Vector3d &rayDir)
{
  CastResult result =
  {
    Eigen::Vector3f(0.0f, 0.0f, 0.0f),
    //Eigen::Vector3f((float)rayDir(0), (float)rayDir(1), 0.0f),
    false,
    Eigen::Vector3d(),
    Eigen::Vector3d()
  };
  
  Eigen::Vector3d end(rayOrigin + (rayDir * RayLength));
  
  if(Eigen::Vector2d(end(0), end(1)).norm() < 50.0)
  {
    result.color = Eigen::Vector3f(1.0f, 0.0f, 1.0f);
    result.didHit = false;
    result.hitPos = end;
    result.newDir = Eigen::Vector3d(0.0, 0.0, -1.0);
  }
  
  return result;
}
#endif

#if 0
void PseudoCast(PixelBuffer &pixBuf, Tracing::Scene &scene, int numIters)
{
  const int HalfWidth = Width / 2;
  const int HalfHeight = Height / 2;
  
  Eigen::Vector3d rayOrigin(0.0, 0.0, 0.0);
  Eigen::Vector3d rayDir;
  Eigen::Vector3f resultColor(0.0f, 0.0f, 0.0f);
  
  for(int y = 0; y < Height; ++y)
  {
    if(y == Height / 2) printDebug = true;
    
    for(int x = 0; x < Width; ++x)
    {
      /* Calculate first rayDir */
      Eigen::Vector3d pixelCoord;
      pixelCoord(0) = (double)(x - HalfWidth) / HalfWidth;
      pixelCoord(1) = -(double)(y - HalfHeight) / HalfHeight;
      pixelCoord(2) = 1.0;
      
      rayDir = (pixelCoord - rayOrigin).normalized();
      
      /* First iteration */
      auto result = DoIteration(scene, rayOrigin, rayDir);
      bool didHit = result.didHit;
      resultColor = result.color;
      if(result.didHit)
      {
        rayOrigin = result.hitPos;
        rayDir = result.newDir;
      }
      else
      {
        rayOrigin = Eigen::Vector3d(0.0, 0.0, 0.0);
        rayDir = (pixelCoord - rayOrigin).normalized();
      }
      
      /* Other iterations */
      int i;
      for(i = 0; i < numIters && didHit; ++i)
      {
        auto result = DoIteration(scene, rayOrigin, rayDir);
        didHit = result.didHit;
        if(result.didHit)
        {
          rayOrigin = result.hitPos;
          rayDir = result.newDir;
          resultColor += result.color;
        }
        else
        {
          rayOrigin = Eigen::Vector3d(0.0, 0.0, 0.0);
          rayDir = (pixelCoord - rayOrigin).normalized();
        }
      }
      
      resultColor /= (i + 1); /* Average color */
      pixBuf.SetPixel(x, y, resultColor);
    }
  }
}
#endif

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

int main(int argc, char **argv)
{
  PixelBuffer myImage(Width, Height);
  
  Tracing::Scene myScene;
  const int numIters = 50; /* Maximum QUALITY Kappa */
  
  std::cout << "Casting...\n";

  /* Materials */
  Tracing::Material redMat(Eigen::Vector3f(0.6f, 0.0f, 0.0f));
  Tracing::Material greenMat(Eigen::Vector3f(0.0f, 0.6f, 0.0f));
  Tracing::Material blueMat(Eigen::Vector3f(0.0f, 0.0f, 0.6f));
  Tracing::Material yellowMat(Eigen::Vector3f(0.6f, 0.6f, 0.0f)); /* Red & green = yellow, right? */
  Tracing::Material pinkMat(Eigen::Vector3f(1.0f, 0.0f, 1.0f));

  /* Entities */
  Tracing::SphereEntity mySphere(Eigen::Vector3d(0.0, 0.0, 20.0), 10.0, redMat);
  Tracing::SphereEntity mySphere2(Eigen::Vector3d(8.0, 0.0, 10.0), 3.0, greenMat);
  Tracing::SphereEntity mySphere3(Eigen::Vector3d(-5.0, 5.0, 5.0), 5.0, blueMat);
  Tracing::SphereEntity mySphere4(Eigen::Vector3d(0.0, -20.0, 15.0), 10.0, yellowMat);
  Tracing::SphereEntity mySphere5(Eigen::Vector3d(-3.0, 0.0, 7.0), 4.0, pinkMat);

  myScene.AddEntity(mySphere);
  myScene.AddEntity(mySphere2);
  myScene.AddEntity(mySphere3);
  myScene.AddEntity(mySphere4);
  myScene.AddEntity(mySphere5);

  /* Lights */
  Tracing::Light myLight(Eigen::Vector3d(0.0, 30.0, 20.0), Eigen::Vector3f(0.0f, 1.0f, 0.0f));
  Tracing::Light myLight2(Eigen::Vector3d(0.0, 0.0, 0.0), Eigen::Vector3f(0.0f, 1.0f, 1.0f));

  myScene.AddLight(myLight);
  myScene.AddLight(myLight2);

  omp_lock_t setPixelLock;
  omp_init_lock(&setPixelLock);

//#pragma omp parallel for
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
      if(result.ObjectHit()) pixelColor /*+*/= result.Color();

      double intensity = result.intensity;
      
      /* We then continue to cast rays, until we hit nothing or the iteration threshold is reached. */
      int doneIters;
      for(doneIters = 0; doneIters < numIters && result.ObjectHit(); ++doneIters)
      {
        ray = result.NextCastRay();
        result = myScene.CastRay(ray, result);
        if(result.ObjectHit()) pixelColor += result.Color() * (float)intensity;

        intensity += result.intensity; // Play with this
        intensity /= 2.0f;
        intensity = std::max(intensity, 0.0);
      }

      /* Average pixel color */
      pixelColor /= (float)(doneIters + 1);

      //omp_set_lock(&setPixelLock);
      myImage.SetPixel(x, y, pixelColor);
      //omp_unset_lock(&setPixelLock);
    }
  }

  omp_destroy_lock(&setPixelLock);

  
  std::cout << "Exporting...\n";
  if(!Exporter::ExportPPM("TestTracer.ppm", myImage))
  {
    std::cout << "Could not export to ppm!\n";
  }
  
  std::cout << "Done\n";
  /*char ch;
  std::cin >> ch;*/
}

