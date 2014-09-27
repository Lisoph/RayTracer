#include "PPMExporter.hpp"

#include <cstdio>
#include <sstream>
#include <iostream>

#include "PixelBuffer.hpp"

namespace RayTracer
{
  namespace Exporter
  {
    static std::string PixelToPPMStr(const Pixel &pixel, int maxColor)
    {
      float alpha = pixel.a / 255.0f;
      Pixel pix(0, (int)(pixel.r * alpha), (int)(pixel.g * alpha), (int)(pixel.b * alpha)); /* Pseudo alpha conversion */
      
      int r = (int)((pix.r / 255.0f) * maxColor);
      int g = (int)((pix.g / 255.0f) * maxColor);
      int b = (int)((pix.b / 255.0f) * maxColor);
      
      std::stringstream ss;
      ss << r << ' ' << g << ' ' << b;
      
      return ss.str();
    }
    
    bool ExportPPM(const std::string &file, const PixelBuffer &pixBuf)
    {
      FILE *fp = fopen(file.c_str(), "w");
      if(!fp) return false;
      
      const int maxColorVal = 255;
      
      /* Write magic number */
      fprintf(fp, "P3\n");
      /* Write image dimension */
      fprintf(fp, "%d %d\n", pixBuf.Width(), pixBuf.Height());
      /* Write max color val */
      fprintf(fp, "%d\n", maxColorVal);
      
      for(int y = 0; y < pixBuf.Height(); ++y)
      {
        for(int x = 0; x < pixBuf.Width(); ++x)
        {
          Nullable<Pixel> pixel = pixBuf.GetPixel(x, y);
          if(pixel.IsNull())
          {
            std::cout << "PPMExporter ERROR: pixel is null!\n";
            fprintf(fp, "EXPORTER ERROR");
            fclose(fp);
            return false;
          }
          
          std::string pixStr = PixelToPPMStr(*pixel, maxColorVal);
          fprintf(fp, " %s ", pixStr.c_str()/*, (x < pixBuf.Width() - 1 ? " " : "")*/);
        }
        
        fprintf(fp, "\n");
      }
      
      fclose(fp);
      
      return true;
    }
  }
}