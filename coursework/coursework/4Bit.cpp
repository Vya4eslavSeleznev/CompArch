#include "4Bit.h"
#include "getOutBufSize.h"
#include "padding.h"

#include <math.h>

void getPaletteFor4Bit(RGBQUAD* Palette)
{
  Palette[0].rgbRed = 0x00;
  Palette[0].rgbGreen = 0x00;
  Palette[0].rgbBlue = 0x00;

  Palette[1].rgbRed = 128;
  Palette[1].rgbGreen = 0;
  Palette[1].rgbBlue = 0;

  Palette[2].rgbRed = 255;
  Palette[2].rgbGreen = 0;
  Palette[2].rgbBlue = 0;

  Palette[3].rgbRed = 255;
  Palette[3].rgbGreen = 0;
  Palette[3].rgbBlue = 255;

  Palette[4].rgbRed = 0;
  Palette[4].rgbGreen = 128;
  Palette[4].rgbBlue = 128;

  Palette[5].rgbRed = 0;
  Palette[5].rgbGreen = 128;
  Palette[5].rgbBlue = 0;

  Palette[6].rgbRed = 0;
  Palette[6].rgbGreen = 255;
  Palette[6].rgbBlue = 0;

  Palette[7].rgbRed = 0;
  Palette[7].rgbGreen = 255;
  Palette[7].rgbBlue = 255;

  Palette[8].rgbRed = 0;
  Palette[8].rgbGreen = 0;
  Palette[8].rgbBlue = 128;

  Palette[9].rgbRed = 128;
  Palette[9].rgbGreen = 0;
  Palette[9].rgbBlue = 128;

  Palette[10].rgbRed = 0;
  Palette[10].rgbGreen = 0;
  Palette[10].rgbBlue = 255;

  Palette[11].rgbRed = 192;
  Palette[11].rgbGreen = 192;
  Palette[11].rgbBlue = 192;

  Palette[12].rgbRed = 128;
  Palette[12].rgbGreen = 128;
  Palette[12].rgbBlue = 128;

  Palette[13].rgbRed = 128;
  Palette[13].rgbGreen = 128;
  Palette[13].rgbBlue = 0;

  Palette[14].rgbRed = 255;
  Palette[14].rgbGreen = 255;
  Palette[14].rgbBlue = 0;

  Palette[15].rgbRed = 255;
  Palette[15].rgbGreen = 255;
  Palette[15].rgbBlue = 255;
}

void pixelConversionTo4Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[])
{
  DWORD RW;
  unsigned char bit = 4;

  for (int i = 0; i < height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * width, &RW, NULL);

    auto pixelMask = new char[2];
    int maskIndex = 0;
    int outIndex = 0;

    for (int j = 0; j < width; ++j)
    {
      auto sum = inBuf[j].rgbtRed + inBuf[j].rgbtGreen + inBuf[j].rgbtBlue;

      pixelMask[maskIndex] = (int)floor(sum * 15 / (3 * 255));

      maskIndex++;

      if (maskIndex == 2)
      {
        BYTE resultByte = 0;

        for (auto k = 0; k < 2; k++)
        {
          if (k != 0)
            resultByte = resultByte << 1;

          resultByte |= pixelMask[k];
        }

        outBuf[outIndex] = resultByte;

        outIndex++;
        maskIndex = 0;
      }
    }

    if (maskIndex != 0)
    {
      BYTE resultByte = 0;

      for (auto k = 0; k < maskIndex; k++)
      {
        if (k != 0)
          resultByte = resultByte << 1;

        resultByte |= pixelMask[k];
      }

      outBuf[outIndex] = (resultByte << 2 - maskIndex);
      maskIndex = 0;
    }

    delete[] pixelMask;

    WriteFile(hOutFile, outBuf, sizeof(BYTE) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}
