#include <bmp_parser.hpp>
#include <cmath>

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
size_t getDeepContent (const std::vector<char>& content, size_t width, size_t height) {
    size_t contentLen = content.size();
    size_t countPixels =  width * height;
    size_t bitsOneByte = 8;
    return contentLen / countPixels * bitsOneByte;
}

size_t getDeepFormat (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x1c, 0x1c + 2);
    return toNumber(chars);
}

// 获取真正渲染使用的色深度
size_t getDeep (size_t deepContent, size_t deepFormat) {
    if (deepContent > 8 || deepContent <= 16) {
        return deepFormat;
    } else {
        return deepContent;
    }
}

size_t getSizeDIB (const std::vector<char>& bmpData) {
    std::vector<char> chars = getSubVector(bmpData, 0x0e, 0x0e + 4);
    return toNumber(chars);
}

size_t getLenPalette (const std::vector<char>& bmpData, size_t deep) {
    std::vector<char> chars = getSubVector(bmpData, 0x2e, 0x2e + 4);
    size_t len = toNumber(chars);
    if (len == 0) {
        len = pow(2, deep);
    }
    return len;
}

std::string colorStrRender (const std::vector<char>& chunk) {
    return "#" + charToHexString(chunk[2]) + charToHexString(chunk[1]) + charToHexString(chunk[0]);
}

std::vector<std::string> getPalette (const std::vector<char>& bmpData, size_t sizeDIB, size_t deep) {
    // 调色板一个色为4个字节
    size_t lenBytesOneColor = 4;
    size_t lenPalette = getLenPalette(bmpData, deep);

    // TODO: 删除这个临时测试
    size_t expectLenPalette = 16;
    ensure(lenPalette, expectLenPalette, "test lenPalette");
    
    // 一般来说调色板的开始是DIB头的结束，0x0e是DIB头的开始
    size_t start = 0x0e + sizeDIB;
    size_t end = start + lenPalette * lenBytesOneColor;
    std::vector<char> l = getSubVector(bmpData, start, end);
    std::vector<std::vector<char>> chunks = chunkList<char>(l, lenBytesOneColor);

    std::vector<std::string> r;
    for (const std::vector<char> chunk : chunks) {
        r.push_back(colorStrRender(chunk));
    }
    return r;
}

std::vector<std::string> getRenderList (const std::vector<char> &content, const std::vector<std::string> &palette) {
    std::vector<char> l;
    for (const char c : content) {
        l.push_back(c & 0x0F);
        l.push_back(c >> 4);
    }
    std::vector<std::string> r;
    for (const char c : l) {
        r.push_back(palette[c]);
    }
    return r;
}

Bmp bmpParser (const std::vector<char>& bmpData) {
    size_t contentStart = getBmpContentStart(bmpData);
    std::vector<char> content = getContent(bmpData, contentStart);
    size_t compression = getBmpCompression(bmpData);
    size_t width = getBmpWidth(bmpData);
    size_t height = getBmpHeight(bmpData);
    size_t deepContent = getDeepContent(content, width, height);
    size_t deepFormat = getDeepFormat(bmpData);
    size_t deep = getDeep(deepContent, deepFormat);
    size_t sizeDIB = getSizeDIB(bmpData);
    std::vector<std::string> palette = getPalette(bmpData, sizeDIB, deep);
    std::vector<std::string> renderList = getRenderList(content, palette);
    // TODO: 添加fixContent，现在得到的content长度是8，不正确，需要变成2
    Bmp bmp = {
        contentStart,
        content,
        compression,
        width,
        height,
        deep,
        sizeDIB,
        palette,
    };

    return bmp;
}