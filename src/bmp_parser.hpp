#ifndef _BMP_VIEWER_BMP_PARSER_DEFINED
#define _BMP_VIEWER_BMP_PARSER_DEFINED

#include <vector>
#include <iostream>
#include <fstream>
#include <bmp_utils.hpp>
struct Bmp {
    size_t contentStart;
};

Bmp bmpParser (const std::vector<char>& bmpData);

#endif