#include "convert.h"
#include "padding.h"
#include "1Bit.h"
#include "4Bit.h"
#include "8Bit.h"
#include "16Bit.h"
#include "getOutBufSize.h"

#include <stdexcept>

bool hasPalette(unsigned char& bit)
{
  return bit == 1 || bit == 4 || bit == 8;
}

DWORD getPixelsSize(int& width, int& height, unsigned char& bit)
{
  auto padding = getPadding(width, bit);

  if (bit == 1)
  {
    return (width * height) / 8 + padding * height;
  }
  else if (bit == 4)
  {
    return (width * height) / 2 + padding * height;
  }
  else if (bit == 8)
  {
    return width * height + padding * height;
  }
  else if (bit == 16)
  {
    return 2 * width * height + padding * height;
  }
  else
  {
    throw new std::invalid_argument("Error");
  }
}

int getPaletteSizeInBytes(int paletteSize, unsigned char& bit)
{
  if (!hasPalette(bit))
    return 0;

  return sizeof(RGBQUAD) * paletteSize;
}

void convert(std::string& fileName, unsigned char& bit)
{
  BITMAPFILEHEADER bmpFileHeader;
  BITMAPINFOHEADER bmpInfoHeader;
  int width, height;
  RGBTRIPLE* inBuf;
  BYTE* outBuf = nullptr;
  Pixel16* outBuf16 = nullptr;
  HANDLE hInputFile, hOutFile;
  DWORD RW;

  int paleteSize = (int)pow(2, bit);
  RGBQUAD* palette = new RGBQUAD[paleteSize];

  auto wstrfileName = std::wstring(fileName.begin(), fileName.end());
  hInputFile = CreateFile(wstrfileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

  if (hInputFile == INVALID_HANDLE_VALUE)
    return;

  std::string result = "Result.bmp";
  auto wstrResult = std::wstring(result.begin(), result.end());
  hOutFile = CreateFile(wstrResult.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

  if (hOutFile == INVALID_HANDLE_VALUE)
  {
    CloseHandle(hInputFile);
    return;
  }

  // Считываем инфу
  ReadFile(hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
  ReadFile(hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

  // Установим указатель на начало растра
  SetFilePointer(hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
  width = bmpInfoHeader.biWidth;
  height = bmpInfoHeader.biHeight;

  // Выделим память
  inBuf = new RGBTRIPLE[width];

  if (bit != 16)
  {
    outBuf = new BYTE[getOutBufSize(width, bit)];
  }
  else
  {
    outBuf16 = new Pixel16[getOutBufSize(width, bit)];
  }

  // Заполним заголовки
  bmpInfoHeader.biBitCount = bit;
  bmpInfoHeader.biSizeImage = getPixelsSize(width, height, bit);
  bmpInfoHeader.biCompression = 0;

  int paletteSizeInBytes = getPaletteSizeInBytes(paleteSize, bit);

  bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + paletteSizeInBytes;
  bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;

  // Запишем заголовки
  WriteFile(hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
  WriteFile(hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

  if (hasPalette(bit))
  {
    if (bit == 1)
    {
      getPaletteFor1Bit(palette);
    }
    else if (bit == 4)
    {
      getPaletteFor4Bit(palette);
    }
    else if (bit == 8)
    {
      getPaletteFor8Bit(palette);
    }
    else
    {
      throw new std::invalid_argument("123");
    }

    WriteFile(hOutFile, palette, paletteSizeInBytes, &RW, NULL);
  }

  if (bit == 1)
  {
    pixelConversionTo1Bit(width, height, hInputFile, hOutFile, outBuf, inBuf, palette);
  }
  else if (bit == 4)
  {
    pixelConversionTo4Bit(width, height, hInputFile, hOutFile, outBuf, inBuf, palette);
  }
  else if (bit == 8)
  {
    pixelConversionTo8Bit(width, height, hInputFile, hOutFile, outBuf, inBuf, palette);
  }
  else if (bit == 16)
  {
    pixelConversionTo16Bit(width, height, hInputFile, hOutFile, outBuf16, inBuf);
  }
  else
  {
    throw new std::invalid_argument("Error");
  }

  delete[] inBuf;
  delete[] outBuf;

  CloseHandle(hInputFile);
  CloseHandle(hOutFile);
}
