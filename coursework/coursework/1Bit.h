#include <windows.h>

void getPaletteFor1Bit(RGBQUAD* Palette);
void pixelConversionTo1Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[]);
