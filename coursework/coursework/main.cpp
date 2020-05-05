#include "convert.h"

using namespace std;

/*struct Pixel16
{
  BYTE s;
  BYTE f;
};*/

/*int getOutBufSize(int& width, unsigned char& bit)
{
  if (bit == 1)
  {
    return (int)ceil(width / (double)8);
  }
  else if (bit == 4)
  {
    return (int)ceil(width / (double)2);
  }
  else if (bit == 8)
  {
    return width;
  }
  else if (bit == 16)
  {
    return width;
  }
  else
  {
    throw new invalid_argument("Error");
  }
}*/

/*int getPadding(int& width, unsigned char& bit)
{
  auto pad = getOutBufSize(width, bit) % 4;

  if (pad == 0)
    return 0;

  return 4 - pad;
}*/

/*bool hasPalette(unsigned char& bit)
{
  return bit == 1 || bit == 4 || bit == 8;
}*/

/*DWORD getPixelsSize(int& width, int& height, unsigned char& bit)
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
    throw new invalid_argument("Error");
  }
}*/

/*int getPaletteSizeInBytes(int paletteSize, unsigned char& bit)
{
  if (!hasPalette(bit))
    return 0;

  return sizeof(RGBQUAD) * paletteSize;
}*/

/*void getPaletteFor8Bit(RGBQUAD* Palette)
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
}*/

/*void getPaletteFor4Bit(RGBQUAD* Palette)
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
}*/

/*void getPaletteFor1Bit(RGBQUAD* Palette)
{
  Palette[0].rgbRed = 0x00;
  Palette[0].rgbGreen = 0x00;
  Palette[0].rgbBlue = 0x00;

  Palette[1].rgbRed = 0xFF;
  Palette[1].rgbGreen = 0xFF;
  Palette[1].rgbBlue = 0xFF;
}*/

/*void addPadding(int& width, HANDLE& hOutFile, unsigned char& bit)
{
  DWORD RW;
  auto paddingSize = getPadding(width, bit);

  for (auto t = 0; t < paddingSize; t++)
  {
    BYTE* padByte = new BYTE;
    *padByte = 0;

    WriteFile(hOutFile, padByte, 1, &RW, NULL);
  }
}*/

/*void pixelConversionTo1Bit(int& width, int& height, HANDLE& hInputFile,HANDLE& hOutFile,
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
}*/

/*void pixelConversionTo4Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
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
}*/

/*void pixelConversionTo8Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
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
}*/

BYTE convertTo5Bit(BYTE& value)
{
  return (BYTE) floor(value * 31 / 255);
}

BYTE convertTo5Bit2(BYTE& value)
{
  return (BYTE)floor(value * 63 / 255);
}

/*void pixelConversionTo16Bit(int& width, int& height, HANDLE& hInputFile, HANDLE& hOutFile,
                            Pixel16* outBuf, RGBTRIPLE* inBuf)
{
  DWORD RW;
  unsigned char bit = 16;

  for (int i = 0; i < height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * width, &RW, NULL);
 
    for (int j = 0; j < width; ++j)
    {
      /*auto red = convertTo5Bit(inBuf[j].rgbtRed);
      auto green = convertTo5Bit2(inBuf[j].rgbtGreen);
      auto blue = convertTo5Bit(inBuf[j].rgbtBlue);*/

     /* auto red = (inBuf[j].rgbtRed * 100 + 505) >> 10;//253
      auto green = (inBuf[j].rgbtGreen * 100 + 505) >> 10;
      auto blue = (inBuf[j].rgbtBlue * 100 + 505) >> 10;

      // blue, green, red 0

      outBuf[j].s = blue;
      outBuf[j].s = (outBuf[j].s << 3) | green & 0x1c;
      outBuf[j].f = green & 0x03;
      outBuf[j].f = (outBuf[j].f << 5) | red;
      outBuf[j].f = (outBuf[j].f << 1) & 0xfe;
    }

    WriteFile(hOutFile, outBuf, sizeof(Pixel16) * getOutBufSize(width, bit), &RW, NULL);
    addPadding(width, hOutFile, bit);
  }
}*/

/*void convert(string& fileName, unsigned char& bit)
{
  BITMAPFILEHEADER bmpFileHeader;
  BITMAPINFOHEADER bmpInfoHeader;
  int width, height;
  RGBTRIPLE* inBuf;
  BYTE* outBuf = nullptr;
  Pixel16* outBuf16 = nullptr;
  HANDLE hInputFile, hOutFile;
  DWORD RW;

  int paleteSize = (int) pow(2, bit);
  RGBQUAD* palette = new RGBQUAD[paleteSize];

  auto wstrfileName = wstring(fileName.begin(), fileName.end());
  hInputFile = CreateFile(wstrfileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

  if (hInputFile == INVALID_HANDLE_VALUE)
    return;

  string result = "Result.bmp";
  auto wstrResult = wstring(result.begin(), result.end());
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
      throw new invalid_argument("123");
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
    throw new invalid_argument("123");
  }

  delete[] inBuf;
  delete[] outBuf;

  CloseHandle(hInputFile);
  CloseHandle(hOutFile);
}*/

int main(int argc, char* argv[])
{
  string fileName = argv[1];
  unsigned char bit = stoi(argv[2]);
  convert(fileName, bit);

  return 0;
}
