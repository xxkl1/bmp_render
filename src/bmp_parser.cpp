#include <bmp_parser.hpp>

size_t getBmpContentStart (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x0a, 0x0a + 4);
    return toNumber(chars);
}

// 获取压缩方式
size_t getBmpCompression (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x1e, 0x1e + 4);
    return toNumber(chars);
}

// 获取宽度
size_t getBmpWidth (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x12, 0x12 + 4);
    return toNumber(chars);
}

// 获取高度
size_t getBmpHeight (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x16, 0x16 + 4);
    return toNumber(chars);
}

// 获取内容区
std::vector<char> getContent (const std::vector<char>& bmpData, size_t contentStart) {
    return getSubVector(bmpData, contentStart, bmpData.size());
}

// 获取色深
size_t getDeep (const std::vector<char>& content, size_t width, size_t height) {
    size_t contentLen = content.size();
    size_t countPixels =  width * height;
    size_t bitsOneByte = 8;
    return contentLen / countPixels * bitsOneByte;
}

size_t getDeepFormat (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x1c, 0x1c + 2);
    return toNumber(chars);
}

Bmp bmpParser (const std::vector<char>& bmpData) {
    size_t contentStart = getBmpContentStart(bmpData);
    std::vector<char> content = getContent(bmpData, contentStart);
    size_t compression = getBmpCompression(bmpData);
    size_t width = getBmpWidth(bmpData);
    size_t height = getBmpHeight(bmpData);
    size_t deep = getDeep(content, width, height);
    size_t deepFormat = getDeepFormat(bmpData);

    Bmp bmp = {
        contentStart,
        content,
        compression,
        width,
        height,
        deep,
        deepFormat,
    };

    return bmp;
}