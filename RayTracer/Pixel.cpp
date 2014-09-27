#include "Pixel.hpp"

namespace RayTracer
{
  Pixel::Pixel()
  : val(0)
  {}
  
  Pixel::Pixel(uint32_t val)
  : val(val)
  {}
  
  Pixel::Pixel(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
  : a(a), r(r), g(g), b(b)
  {}
  
  Pixel::Pixel(const Pixel &other)
  : val(other.val)
  {}
  
  Pixel::Pixel(const Eigen::Vector3f &color)
  : Pixel(255, (int)(color(0) * 255), (int)(color(1) * 255), (int)(color(2) * 255))
  {}
}