#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

int getPadding(int& bit, int& Width)
{
  int padding;

  if (bit == 1)
  {
    padding = (5 * Width) % 4;
  }
  else if (bit == 4)
  {
    padding = (4 * Width) % 4;
  }
  else if (bit == 8)
  {
    padding = (3 * Width) % 4;
  }
  else if (bit == 16)
  {
    padding = (2 * Width) % 4;
  }
  else if (bit == 24)
  {
    padding = Width % 4;
  }

  return padding;
}

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

void getPaletteFor4Bit(RGBQUAD* Palette)
{
  Palette[0].rgbRed = 0x00;
  Palette[0].rgbGreen = 0x00;
  Palette[0].rgbBlue = 0x00;

  Palette[1].rgbRed = 0x14;
  Palette[1].rgbGreen = 0x14;
  Palette[1].rgbBlue = 0x14;

  Palette[2].rgbRed = 0x20;
  Palette[2].rgbGreen = 0x20;
  Palette[2].rgbBlue = 0x20;

  Palette[3].rgbRed = 0x2c;
  Palette[3].rgbGreen = 0x2c;
  Palette[3].rgbBlue = 0x2c;

  Palette[4].rgbRed = 0x38;
  Palette[4].rgbGreen = 0x38;
  Palette[4].rgbBlue = 0x38;

  Palette[5].rgbRed = 0x45;
  Palette[5].rgbGreen = 0x45;
  Palette[5].rgbBlue = 0x45;

  Palette[6].rgbRed = 0x51;
  Palette[6].rgbGreen = 0x51;
  Palette[6].rgbBlue = 0x51;

  Palette[7].rgbRed = 0x61;
  Palette[7].rgbGreen = 0x61;
  Palette[7].rgbBlue = 0x61;

  Palette[8].rgbRed = 0x71;
  Palette[8].rgbGreen = 0x71;
  Palette[8].rgbBlue = 0x71;

  Palette[9].rgbRed = 0x82;
  Palette[9].rgbGreen = 0x82;
  Palette[9].rgbBlue = 0x82;

  Palette[10].rgbRed = 0x92;
  Palette[10].rgbGreen = 0x92;
  Palette[10].rgbBlue = 0x92;

  Palette[11].rgbRed = 0xa2;
  Palette[11].rgbGreen = 0xa2;
  Palette[11].rgbBlue = 0xa2;

  Palette[12].rgbRed = 0xb6;
  Palette[12].rgbGreen = 0xb6;
  Palette[12].rgbBlue = 0xb6;

  Palette[13].rgbRed = 0xcb;
  Palette[13].rgbGreen = 0xcb;
  Palette[13].rgbBlue = 0xcb;

  Palette[14].rgbRed = 0xe3;
  Palette[14].rgbGreen = 0xe3;
  Palette[14].rgbBlue = 0xe3;

  Palette[15].rgbRed = 0xff;
  Palette[15].rgbGreen = 0xff;
  Palette[15].rgbBlue = 0xff;
}

void getPaletteFor1Bit(RGBQUAD* Palette)
{
  Palette[0].rgbRed = 0x00;
  Palette[0].rgbGreen = 0x00;
  Palette[0].rgbBlue = 0x00;

  Palette[1].rgbRed = 0xFF;
  Palette[1].rgbGreen = 0xFF;
  Palette[1].rgbBlue = 0xFF;
}

void pixelConversionFor8Bit(BYTE* outBuf, RGBTRIPLE* inBuf, int& j)
{
  outBuf[j] = 32 * (inBuf[j].rgbtRed / 32) + 4 * (inBuf[j].rgbtGreen / 32) + inBuf[j].rgbtBlue / 64;
}

void pixelConversationFor4Bit(BYTE* outBuf, RGBTRIPLE* inBuf, int& j)
{
  outBuf[j] = 0.01 * inBuf[j].rgbtRed + 0.1 * inBuf[j].rgbtGreen + 0.01 * inBuf[j].rgbtBlue;
}

void pixelConversationFor1Bit(BYTE* outBuf, RGBTRIPLE* inBuf, int& j, int& error)
{
  auto average = (inBuf[j].rgbtRed + inBuf[j].rgbtGreen + inBuf[j].rgbtBlue) / 3;

  outBuf[j] = average > 128 ? 1 : 0;
}

void pixelConversion(int& Width, int& Height, HANDLE& hInputFile, HANDLE& hOutFile,
                     BYTE* outBuf, RGBTRIPLE* inBuf, DWORD& RW, RGBQUAD Palette[],
                     int& size, int& padding)
{
  for (int i = 0; i < Height; ++i)
  {
    ReadFile(hInputFile, inBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);

    auto mask = new char[8];
    int maskIndex = 0;
    int index = 0;

    for (int j = 0; j < Width; ++j)
    {
      auto average = (inBuf[j].rgbtRed + inBuf[j].rgbtGreen + inBuf[j].rgbtBlue) / 3;

      mask[maskIndex] = average > 128 ? 1 : 0;

      maskIndex++;

      if (maskIndex == 8)
      {
        BYTE test = 0;

        for (auto k = 0; k < 8; k++)
        {
          if (k != 0)
            test = test << 1;

          test |= (mask[k] == 1 ? 0x01 : 0x00);
        }

        outBuf[index] = test;
        //outBuf[index] = 0;
        index++;
        maskIndex = 0;
      }
      
      //pixelConversationFor1Bit(outBuf, inBuf, j, error);
      //out << (short)outBuf[j];
     
      //out << "=======================================================================" << endl;
      //out << (short)inBuf[j].rgbtRed << " " << (short)inBuf[j].rgbtGreen << " " << (short)inBuf[j].rgbtBlue << endl;
    }

    if (maskIndex != 0)
    {
      BYTE test = 0;

      for (auto k = 0; k < maskIndex; k++)
      {
        if (k != 0)
          test = test << 1;

        test |= (mask[k] == 1 ? 0x01 : 0x00);
      }

      outBuf[index] = (test << 8 - maskIndex);
      maskIndex = 0;
    }

    delete[] mask;

    WriteFile(hOutFile, outBuf, sizeof(BYTE) * (int)ceil(Width / (double)8), &RW, NULL);

    auto pad = ((int)ceil(Width / (double)8)) % 4;

    if (pad != 0)
      pad = 4 - pad;

    for (auto t = 0; t < pad; t++)
    {
      BYTE* padByte = new BYTE;
      *padByte = 0;
      WriteFile(hOutFile, padByte, 1, &RW, NULL);
    }

    // Пишем мусор для выравнивания
    
    //SetFilePointer(hInputFile, Width % 4, NULL, FILE_CURRENT);
  }
}

void withPalette(string fileName)
{
  //string fileName;
  BITMAPFILEHEADER bmpFileHeader;
  BITMAPINFOHEADER bmpInfoHeader;
  int Width, Height;
  RGBTRIPLE* inBuf;
  BYTE* outBuf;
  HANDLE hInputFile, hOutFile;
  DWORD RW;

  int bit = 1;
  int size = pow(2, bit);
  RGBQUAD* Palette = new RGBQUAD[size];

  //cout << "Enter the full name, please: ";
  //cin >> fileName;
  auto temp = wstring(fileName.begin(), fileName.end());
  hInputFile = CreateFile(temp.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

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
  Width = bmpInfoHeader.biWidth;
  Height = bmpInfoHeader.biHeight;

  // Выделим память
  inBuf = new RGBTRIPLE[Width];
  auto outBufSize = (int)ceil(Width / (double)8);
  outBuf = new BYTE[outBufSize];

  // Заполним заголовки
  int padding = getPadding(bit, Width);
 
  bmpInfoHeader.biBitCount = bit;
  bmpInfoHeader.biSizeImage = (Width * Height) / 8 + ((int)ceil(Width / (double)8) % 4 * Height);
  bmpInfoHeader.biCompression = 0;

  bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4 * size;
  bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bmpInfoHeader.biSizeImage;
  
  // Запишем заголовки
  WriteFile(hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
  WriteFile(hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

  // Палитра
  //getPalette(bmpInfoHeader.biBitCount, size, Palette, maxEl, out);
  getPaletteFor1Bit(Palette);

  WriteFile(hOutFile, Palette, size * sizeof(RGBQUAD), &RW, NULL);

  // Начнем преобразовывать
  pixelConversion(Width, Height, hInputFile, hOutFile,
                  outBuf, inBuf, RW, Palette, size, padding);

  delete[] inBuf;
  delete[] outBuf;
  CloseHandle(hInputFile);
  CloseHandle(hOutFile);

  //cout << "Updating has come to the end successfully!";
}

void withoutPalette()
{

}

int main()
{
  string fileName = "C:\\University\\Repositories\\CompArch\\coursework\\coursework\\1.bmp";
  
  withPalette(fileName);

  return 0;
}
