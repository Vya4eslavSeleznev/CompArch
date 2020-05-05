#include "16Bit.h"
#include "getOutBufSize.h"
#include "padding.h"

#include <math.h>

void pixelConversionTo16Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  Pixel16* outBuf, RGBTRIPLE* inBuf)
{
  DWORD RW;
  unsigned char bit = 16;

  for (int i = 0; i < height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * width, &RW, NULL);

    for (int j = 0; j < width; ++j)
    {
      auto red = (BYTE)round(inBuf[j].rgbtRed * (double)31 / (double)255);
      auto green = (BYTE)round(inBuf[j].rgbtGreen * (double)31 / (double)255);
      auto blue = (BYTE)round(inBuf[j].rgbtBlue * (double)31 / (double)255);

      auto greenFirst = (green & 0x07) << 5;
      auto greenSecond = (green >> 3);

      outBuf[j].first = blue;
      outBuf[j].first |= greenFirst;

      outBuf[j].second = red;
      outBuf[j].second = (outBuf[j].second << 2);
      outBuf[j].second |= greenSecond;
    }

    WriteFile(hOutFile, outBuf, sizeof(Pixel16) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}
