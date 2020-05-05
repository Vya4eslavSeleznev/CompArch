#include "convert.h"

int main(int argc, char* argv[])
{
  std::string fileName = argv[1];
  unsigned char bit = std::stoi(argv[2]);
  convert(fileName, bit);

  return 0;
}
