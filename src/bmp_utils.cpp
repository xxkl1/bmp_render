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

/**
 * 截取std::vector<char>，star - end，不包含end
 */
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
template void ensure<size_t>(size_t result, size_t expect, const char* message);

// TODO: 想办法去掉泛型的使用
template <>
void ensure<std::vector<std::string>>(std::vector<std::string> result, std::vector<std::string> expect, const char* message) {
    if (result.size() != expect.size()) {
        std::cout << message << " test fail" << std::endl;
        return;
    }
    for (size_t i = 0; i < result.size(); ++i) {
        std::string curExpect = expect[i];
        std::string curResult = result[i];
        if (curExpect != curResult) {
            // TODO: 去除重复代码
            std::cout << message << " test fail" << std::endl;
            return;
        }
    }
    std::cout << message << " tes success" << std::endl;
}

template <>
void ensure<std::vector<std::vector<char>>>(std::vector<std::vector<char>> result, std::vector<std::vector<char>> expect, const char* message) {
    if (result.size() != expect.size()) {
        std::cout << message << " test size std::vector<std::vector<char>> fail" << std::endl;
        return;
    }
    for (size_t i = 0; i < result.size(); ++i) {
        std::vector<char> curExpect = expect[i];
        std::vector<char> curResult = result[i];
        if (curResult.size() != curExpect.size()) {
            std::cout << message << " test std::vector<char> fail" << std::endl;
            return;
        }
        for (size_t i = 0; i < curResult.size(); ++i) {
            if (curResult[i] != curExpect[i]) {
                std::cout << message << " test fail" << std::endl;
                return;
            }
        }
    }
    std::cout << message << " tes success" << std::endl;
}

template<typename T>
std::vector<std::vector<T>> chunkList (const std::vector<T>& list, size_t lenChunk) {
    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < list.size(); ++i) {
        if (i % lenChunk == 0) {
            std::vector<T> chunk = getSubVector(list, i, i + lenChunk);
            result.push_back(chunk);
        }
    }
    return result;
}
template std::vector<std::vector<char>> chunkList (const std::vector<char>& list, size_t lenChunk);

std::string charToHexString(char c) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
    return ss.str();
}
