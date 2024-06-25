#include <bmp_utils.hpp>

std::vector<char> readFile (const std::string& filePath) {
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile) {
        throw std::invalid_argument("can't open file");
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    return buffer;
}