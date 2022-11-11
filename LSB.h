#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <bitset>

class LSB {
private:
    void skipHeader(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out, size_t headerSize);
    void skipHeader(std::basic_ifstream<char> &in, size_t headerSize);
    void sizeToFile(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out, uint64_t textSize);
    uint64_t sizeFromFile(std::basic_ifstream<char> &in);
public:
    void injectionInBitmap(const std::string &filename, const std::string &encodedFilename, const std::string &text);
    std::string withdrawFromFile(const std::string &encodedFilename);
};
