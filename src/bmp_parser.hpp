#ifndef _BMP_VIEWER_BMP_PARSER_DEFINED
#define _BMP_VIEWER_BMP_PARSER_DEFINED

#include <vector>
#include <iostream>
#include <fstream>
#include <bmp_utils.hpp>
struct Bmp {
    size_t contentStart;
    std::vector<char> content;
    size_t compression;
    size_t width;
    size_t height;
    size_t deep;
    size_t deepFormat;
};

Bmp bmpParser (const std::vector<char>& bmpData);

#endif