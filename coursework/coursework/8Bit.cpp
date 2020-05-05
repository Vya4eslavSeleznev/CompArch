#include "8Bit.h"
#include "getOutBufSize.h"
#include "padding.h"

void getPaletteFor8Bit(RGBQUAD* Palette)
{
  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      for (int k = 0; k < 4; ++k)
      {
        Palette[i * 32 + j * 4 + k].rgbRed = i * 32;
        Palette[i * 32 + j * 4 + k].rgbGreen = j * 32;
        Palette[i * 32 + j * 4 + k].rgbBlue = k * 64;
      }
    }
  }
}

void pixelConversionTo8Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[])
{
  DWORD RW;
  unsigned char bit = 8;

  for (int i = 0; i < height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * width, &RW, NULL);

    for (int j = 0; j < width; ++j)
    {
      outBuf[j] = 32 * (inBuf[j].rgbtRed / 32) + 4 * (inBuf[j].rgbtGreen / 32) + inBuf[j].rgbtBlue / 64;
    }

    WriteFile(hOutFile, outBuf, sizeof(BYTE) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}
