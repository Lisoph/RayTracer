#include "PixelBuffer.hpp"

#include <cstdlib>

namespace RayTracer
{
  PixelBuffer::PixelBuffer()
  : width(0), height(0), pixels()
  {}
  
  PixelBuffer::PixelBuffer(int width, int height)
  : width(width), height(height), pixels(new Pixel[width * height])
  {}
  
  PixelBuffer::~PixelBuffer() {}
  
  Nullable<Pixel> PixelBuffer::GetPixel(int x, int y) const
  {
    if(x >= 0 && x < width && y >= 0 && y < height)
      return pixels[Index(x, y)];
    return nullptr;
  }
  
  bool PixelBuffer::SetPixel(int x, int y, const Pixel &pixel)
  {
    if(x >= 0 && x < width && y >= 0 && y < height)
    {
      pixels[Index(x, y)] = pixel;
      return true;
    }
    
    return false;
  }
}