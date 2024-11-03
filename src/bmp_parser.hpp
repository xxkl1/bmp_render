#ifndef _BMP_VIEWER_BMP_PARSER_DEFINED
#define _BMP_VIEWER_BMP_PARSER_DEFINED

#include <vector>
#include <iostream>
#include <fstream>
#include <bmp_utils.hpp>
struct BMP {
    size_t contentStart;
    std::vector<u_char> content;
    size_t compression;
    size_t width;
    size_t height;
    size_t deep;
    size_t sizeDIB;
    std::vector<std::vector<u_char>> palette;
    std::vector<std::vector<u_char>> pixels;
};

BMP bmpParser (const std::vector<u_char>& bmpData);

#endif