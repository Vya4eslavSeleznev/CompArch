#include <windows.h>

void getPaletteFor8Bit(RGBQUAD* Palette);
void pixelConversionTo8Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[]);
