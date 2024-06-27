#include <bmp_parser.hpp>

size_t getBmpContentStart (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x0a, 0x0a+4);
    return toNumber(chars);
}

void getBmpContent (const std::vector<char>& bmpData) {
}

Bmp bmpParser (const std::vector<char>& bmpData) {
    size_t contentStart = getBmpContentStart(bmpData);

    Bmp bmp = {
        contentStart,
    };
    return bmp;
}