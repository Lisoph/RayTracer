#pragma once

#include <cstdint>

#include <Eigen/Eigen>

namespace RayTracer
{
  struct Pixel
  {
    union
    {
      uint32_t val;
      
      struct
      {
        uint8_t a, r, g, b;
      };
    };
    
    Pixel();
    Pixel(uint32_t val);
    Pixel(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    Pixel(const Pixel &other);
    Pixel(const Eigen::Vector3f &color);
    
    Pixel &operator = (const Pixel &rhs)
    {
      this->val = rhs.val;
      return *this;
    }
  };
}