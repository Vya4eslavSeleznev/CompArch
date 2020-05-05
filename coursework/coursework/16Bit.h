#include <windows.h>

struct Pixel16
{
  BYTE first;
  BYTE second;
};

void pixelConversionTo16Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  Pixel16* outBuf, RGBTRIPLE* inBuf);
