#include "16Bit.h"
#include "getOutBufSize.h"
#include "padding.h"

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
      auto red = (inBuf[j].rgbtRed * 100 + 505) >> 10;//253
      auto green = (inBuf[j].rgbtGreen * 100 + 505) >> 10;
      auto blue = (inBuf[j].rgbtBlue * 100 + 505) >> 10;

      outBuf[j].first = blue;
      outBuf[j].first = (outBuf[j].first << 3) | green & 0x1c;
      outBuf[j].second = green & 0x03;
      outBuf[j].second = (outBuf[j].second << 5) | red;
      outBuf[j].second = (outBuf[j].second << 1) & 0xfe;
    }

    WriteFile(hOutFile, outBuf, sizeof(Pixel16) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}
