#include <string>
#include <windows.h>

bool hasPalette(unsigned char& bit);
DWORD getPixelsSize(int& width, int& height, unsigned char& bit);
int getPaletteSizeInBytes(int paletteSize, unsigned char& bit);
void convert(std::string& fileName, unsigned char& bit);
