#include <windows.h>

void getPaletteFor4Bit(RGBQUAD* Palette);
void pixelConversionTo4Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[]);
