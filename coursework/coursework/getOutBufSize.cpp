#include <math.h>
#include <stdexcept>

#include "getOutBufSize.h"

int getOutBufSize(int& width, unsigned char& bit)
{
  if (bit == 1)
  {
    return (int)ceil(width / (double)8);
  }
  else if (bit == 4)
  {
    return (int)ceil(width / (double)2);
  }
  else if (bit == 8)
  {
    return width;
  }
  else if (bit == 16)
  {
    return width;
  }
  else
  {
    throw new std::invalid_argument("Error");
  }
}
