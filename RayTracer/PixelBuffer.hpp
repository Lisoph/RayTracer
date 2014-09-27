#pragma once

#include <memory>

#include "Pixel.hpp"
#include "Nullable.hpp"

namespace RayTracer
{
  class PixelBuffer
  {
  protected:
    int width, height;
    std::unique_ptr<Pixel[]> pixels;
  public:
    PixelBuffer();
    PixelBuffer(int width, int height);
    ~PixelBuffer();
  public:
    int Width() const { return width; }
    int Height() const { return height; }
  public:
    int Index(int x, int y) const { return width * y + x; }
    
    Nullable<Pixel> GetPixel(int x, int y) const;
    bool SetPixel(int x, int y, const Pixel &pixel);
  };
}