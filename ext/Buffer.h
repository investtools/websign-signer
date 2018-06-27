#pragma once

#include <windows.h>

template <class T>
class Buffer
{
public:
  T memory = NULL;
  DWORD size = 0;

  Buffer() {}
  Buffer(Buffer &other)
  {
    size = other.size;
    if (other.memory)
    {
      allocate();
      memcpy(memory, other.memory, size);
    }
  }

  void allocate()
  {
    deallocate();
    if (!(memory = (T)malloc(size)))
    {
      throw std::bad_alloc();
    }
  }

  void deallocate()
  {
    if (memory)
    {
      free(memory);
      memory = NULL;
    }
  }

  ~Buffer()
  {
    deallocate();
  }
};
