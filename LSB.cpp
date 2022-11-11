#include "LSB.h"

void LSB::skipHeader(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out, size_t headerSize) {
    auto *temp = new uint8_t[headerSize];
    in.read(reinterpret_cast<char *>(temp), headerSize);
    out.write(reinterpret_cast<const char *>(temp), headerSize);
    delete[] temp;
}
void LSB::skipHeader(std::basic_ifstream<char> &in, size_t headerSize) {
    auto *temp = new uint8_t[headerSize];
    in.read(reinterpret_cast<char *>(temp), headerSize);
    delete[] temp;
}

void LSB::sizeToFile(std::basic_ifstream<char> &in, std::basic_ofstream<char> &out, uint64_t textSize) {
    std::bitset<64> bits(textSize);
    for (int i = 0; i < 64; ++i) {
        uint64_t buffer;
        in.read(reinterpret_cast<char *>(&buffer), 8);
        std::bitset<64> bitsToFile(buffer);
        bitsToFile[0] = bits[i];
        uint64_t bufferToFile = bitsToFile.to_ullong();
        out.write(reinterpret_cast<char *>(&bufferToFile), 8);
    }
}

uint64_t LSB::sizeFromFile(std::basic_ifstream<char> &in) {
    std::bitset<64> bits(0);
    for (int i = 0; i < 64; ++i) {
        uint64_t buffer;
        in.read(reinterpret_cast<char *>(&buffer), 8);
        std::bitset<64> bitsToFile(buffer);
        bits[i] = bitsToFile[0];
    }
    return (uint64_t)bits.to_ullong();
}

void LSB::injectionInBitmap(const std::string &filename, const std::string &encodedFilename, const std::string &text) {
    std::ifstream in;
    std::ofstream out;
    try {
        in.open(filename, std::ios::binary);
        out.open(encodedFilename, std::ios::binary);
        skipHeader(in, out, 1078);
        uint64_t textSize = text.size();
        sizeToFile(in, out, textSize);

        for (auto c : text) {
            std::bitset<8> bits(c);
            for (int i = 0; i < 8; ++i) {
                uint64_t buffer;
                in.read(reinterpret_cast<char *>(&buffer), 8);
                std::bitset<64> bitsToFile(buffer);
                bitsToFile[0] = bits[i];
                uint64_t bufferToFile = bitsToFile.to_ullong();
                out.write(reinterpret_cast<char *>(&bufferToFile), 8);
            }
        }
        while (!in.eof()) {
            uint8_t temp;
            in.read(reinterpret_cast<char *>(&temp), sizeof temp);
            out.write(reinterpret_cast<const char *>(&temp), sizeof temp);
        }
        in.close();
        out.close();
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "Exception opening/reading in";
    }
}

std::string LSB::withdrawFromFile(const std::string &encodedFilename) {
    std::ifstream in;
    std::string result;
    try {
        in.open(encodedFilename, std::ios::binary);
        skipHeader(in, 1078);
        uint64_t size = sizeFromFile(in);
        for (uint64_t i = 0; i < size; ++i) {
            std::bitset<8> bits(0);
            for (int j = 0; j < 8; ++j) {
                uint64_t buffer;
                in.read(reinterpret_cast<char *>(&buffer), 8);
                std::bitset<64> bitsFromFile(buffer);
                bits[j] = bitsFromFile[0];
            }
            char next_character = (char)bits.to_ulong();
            result.append(1, next_character);
        }
//        while (!in.eof()) {
//            uint8_t temp;
//            in.read(reinterpret_cast<char *>(&temp), sizeof temp);
//        }
        in.close();
        return result;
    }
    catch (const std::ifstream::failure& e) {
        std::cout << "Exception opening/reading in";
    }
}
