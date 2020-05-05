#include <windows.h>

#include "padding.h"
#include "getOutBufSize.h"

int getPadding(int& width, unsigned char& bit)
{
  auto pad = getOutBufSize(width, bit) % 4;

  if (pad == 0)
    return 0;

  return 4 - pad;
}

void addPadding(int& width, HANDLE& hOutFile, unsigned char& bit)
{
  DWORD RW;
  auto paddingSize = getPadding(width, bit);

  for (auto t = 0; t < paddingSize; t++)
  {
    BYTE* padByte = new BYTE;
    *padByte = 0;

    WriteFile(hOutFile, padByte, 1, &RW, NULL);
  }
}
