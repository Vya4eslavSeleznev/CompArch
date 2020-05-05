#include "1Bit.h"
#include "getOutBufSize.h"
#include "padding.h"

void getPaletteFor1Bit(RGBQUAD* Palette)
{
  Palette[0].rgbRed = 0x00;
  Palette[0].rgbGreen = 0x00;
  Palette[0].rgbBlue = 0x00;

  Palette[1].rgbRed = 0xFF;
  Palette[1].rgbGreen = 0xFF;
  Palette[1].rgbBlue = 0xFF;
}

void pixelConversionTo1Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
  BYTE* outBuf, RGBTRIPLE* inBuf, RGBQUAD Palette[])
{
  DWORD RW;
  unsigned char bit = 1;

  for (int i = 0; i < height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * width, &RW, NULL);

    auto pixelMask = new char[8];
    int maskIndex = 0;
    int outIndex = 0;

    for (int j = 0; j < width; ++j)
    {
      auto average = (inBuf[j].rgbtRed + inBuf[j].rgbtGreen + inBuf[j].rgbtBlue) / 3;

      pixelMask[maskIndex] = average > 128 ? 1 : 0;

      maskIndex++;

      if (maskIndex == 8)
      {
        BYTE resultByte = 0;

        for (auto k = 0; k < 8; k++)
        {
          if (k != 0)
            resultByte = resultByte << 1;

          resultByte |= (pixelMask[k] == 1 ? 0x01 : 0x00);
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

        resultByte |= (pixelMask[k] == 1 ? 0x01 : 0x00);
      }

      outBuf[outIndex] = (resultByte << 8 - maskIndex);
      maskIndex = 0;
    }

    delete[] pixelMask;

    WriteFile(hOutFile, outBuf, sizeof(BYTE) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}
