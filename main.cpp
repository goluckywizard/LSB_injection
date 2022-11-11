#include <iostream>
#include "LSB.h"

int main() {
    LSB entity;
    entity.injectionInBitmap("image.bmp", "lsb image.bmp", "text to test steganography");
    std::cout << entity.withdrawFromFile("lsb image.bmp");
    return 0;
}
