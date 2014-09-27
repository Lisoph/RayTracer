#pragma once

namespace RayTracer
{
  template <typename T>
  class Nullable
  {
  protected:
    T data;
    bool isNull;
  public:
    Nullable(const T &data)
    : data(data), isNull(false)
    {}
    
    Nullable(std::nullptr_t np)
    : data(), isNull(true)
    {}
  public:
    bool IsNull() const { return isNull; }
  public:
    T &Data() { return data; }
  };
  
  template <typename T>
  T &operator * (Nullable<T> &nullable)
  {
    return nullable.Data();
  }
}