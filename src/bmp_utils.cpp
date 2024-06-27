#include <bmp_utils.hpp>
#include <bmp_parser.hpp>

template<typename... Args>
void log(Args&&... args) {
    (std::cout << ... << args) << std::endl;
}

std::vector<char> readFile (const std::string& filePath) {
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile) {
        throw std::invalid_argument("can't open file");
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    return buffer;
}

std::vector<char> getSubVector(const std::vector<char>& vec, size_t start, size_t end) {
    if (start >= end || end > vec.size()) {
        throw std::out_of_range("Invalid range.");
    }
    return std::vector<char>(vec.begin() + start, vec.begin() + end);
}

/**
 * @brief 小端序，char 数据列表转 整型
 * 例如: [0x01, 0x02, 0x03, 0x04] -> 0x04030201
 * @param chars 小端序，char 数据列表
 * @return size_t
 */
size_t toNumber (const std::vector<char>& chars) {
    size_t result = 0;
    for (size_t i = 0; i < chars.size(); ++i) {
        char cur = chars[i];
        u_char curUchar = static_cast<u_char>(cur);
        size_t curInt = static_cast<size_t>(curUchar);
        size_t offset = i * 8;
        result = result | (curInt << offset);
    }
    return result;
}

template<typename T>
void ensure(T result, T expect, const char* message) {
    if (result == expect) {
        std::cout << message << " test success" << std::endl;
    } else {
        std::cout << message << " test fail" << std::endl;
    }
}
template void ensure<size_t>(size_t, size_t, const char* message);
