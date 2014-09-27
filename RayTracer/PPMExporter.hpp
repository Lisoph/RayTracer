#pragma once

#include <string>

namespace RayTracer
{
  class PixelBuffer;
  
  namespace Exporter
  {
    bool ExportPPM(const std::string &file, const PixelBuffer &pixBuf);
  }
}