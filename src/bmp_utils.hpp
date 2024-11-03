#ifndef _BMP_VIEWER_BMP_UTILS_DEFINED
#define _BMP_VIEWER_BMP_UTILS_DEFINED

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<u_char> readFile (const std::string& filePath);

std::vector<u_char> getSubVector(const std::vector<u_char>& vec, size_t start, size_t end);

size_t toNumber (const std::vector<u_char>& chars);

template<typename T>
void ensure(T result, T expect, const char* message);

template<typename T>
std::vector<std::vector<T>> chunkList (const std::vector<T>& list, size_t lenChunk);

std::string charToHexString(u_char c);

#endif