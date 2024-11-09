#include <bmp_parser.hpp>
#include <cmath>

size_t getBmpContentStart (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x0a, 0x0a + 4);
    return toNumber(chars);
}

// 获取压缩方式
size_t getBmpCompression (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x1e, 0x1e + 4);
    return toNumber(chars);
}

// 获取宽度
size_t getBmpWidth (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x12, 0x12 + 4);
    return toNumber(chars);
}

// 获取高度
size_t getBmpHeight (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x16, 0x16 + 4);
    return toNumber(chars);
}

// 获取内容区
std::vector<u_char> getContent (const std::vector<u_char>& bmpData, size_t contentStart) {
    return getSubVector(bmpData, contentStart, bmpData.size());
}

// 获取色深
size_t getDeepContent (const std::vector<u_char>& content, size_t width, size_t height) {
    size_t contentLen = content.size();
    size_t countPixels =  width * height;
    size_t bitsOneByte = 8;
    return contentLen / countPixels * bitsOneByte;
}

size_t getDeepFormat (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x1c, 0x1c + 2);
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

size_t getSizeDIB (const std::vector<u_char>& bmpData) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x0e, 0x0e + 4);
    return toNumber(chars);
}

size_t getLenPalette (const std::vector<u_char>& bmpData, size_t deep) {
    std::vector<u_char> chars = getSubVector(bmpData, 0x2e, 0x2e + 4);
    size_t len = toNumber(chars);
    if (len == 0) {
        len = pow(2, deep);
    }
    return len;
}

std::string colorStrRender (const std::vector<u_char>& chunk) {
    return "#" + charToHexString(chunk[2]) + charToHexString(chunk[1]) + charToHexString(chunk[0]);
}

/**
 * 返回格式为[[b, g, r, x], ....]，x位无意义，标准bmp不支持透明度
 */
std::vector<std::vector<u_char>> getPalette (const std::vector<u_char>& bmpData, size_t sizeDIB, size_t deep) {
    std::vector<std::vector<u_char>> r;
    if (deep == 4 || deep == 8) {
        // 调色板一个色为4个字节
        size_t lenBytesOneColor = 4;
        size_t lenPalette = getLenPalette(bmpData, deep);
        
        // 一般来说调色板的开始是DIB头的结束，0x0e是DIB头的开始
        size_t start = 0x0e + sizeDIB;
        size_t end = start + lenPalette * lenBytesOneColor;
        std::vector<u_char> l = getSubVector(bmpData, start, end);
        std::vector<std::vector<u_char>> chunks = chunkList<u_char>(l, lenBytesOneColor);

        for (const std::vector<u_char> chunk : chunks) {
            r.push_back(chunk);
        }
        return r;
    }
    return r;
}

std::vector<std::vector<u_char>> getPixels (const std::vector<u_char> &content, size_t deep, std::vector<std::vector<u_char>> &palette) {
    if (deep == 4) {
        std::vector<u_char> l;
        for (const u_char c : content) {
            l.push_back(c >> 4);
            l.push_back(c & 0x0F);
        }
        std::vector<std::vector<u_char>> r;
        for (const u_char c : l) {
            r.push_back(palette[c]);
        }
        return r;
    } else if (deep == 8) {
        std::vector<std::vector<u_char>> r;
        for (const u_char c : content) {
            r.push_back(palette[c]);
        }
        return r;
    } else {
        return chunkList<u_char>(content, deep / 8);
    }
}

std::vector<u_char> fixContent (const std::vector<u_char>& content, const size_t deep, const size_t deepFormat, const size_t width, const size_t height) {
    if (deep > 8 && deep <= 16 && deep != deepFormat) {
        size_t pixelsCount = width * height;
        float lenBytesOnePixel = static_cast<float>(deepFormat) / 8;
        size_t lenBytesNeed = static_cast<size_t>(std::floor(lenBytesOnePixel * pixelsCount));
        size_t lenC = content.size();
        size_t lenDiff = lenC - lenBytesNeed;
        lenDiff = lenDiff / 2;
        std::vector<std::vector<u_char>> contentChunk = chunkList<u_char>(content, lenC / 2);
        std::vector<u_char> r;
        for (const std::vector<u_char> c : contentChunk) {
            std::vector<u_char> contentCut(c.begin(), c.begin() + (c.size() - lenDiff));
            r.insert(r.end(), contentCut.begin(), contentCut.end());
        }
        return r;
    } else {
        return content;
    }
}

BMP bmpParser (const std::vector<u_char>& bmpData) {
    size_t contentStart = getBmpContentStart(bmpData);
    std::vector<u_char> content = getContent(bmpData, contentStart);
    size_t compression = getBmpCompression(bmpData);
    size_t width = getBmpWidth(bmpData);
    size_t height = getBmpHeight(bmpData);
    size_t deepContent = getDeepContent(content, width, height);
    size_t deepFormat = getDeepFormat(bmpData);
    size_t deep = getDeep(deepContent, deepFormat);
    size_t sizeDIB = getSizeDIB(bmpData);
    std::vector<std::vector<u_char>> palette = getPalette(bmpData, sizeDIB, deep);
    std::vector<u_char> contentFixed = fixContent(content, deepContent, deepFormat, width, height);
    std::vector<std::vector<u_char>> pixels = getPixels(contentFixed, deep, palette);
    BMP bmp = {
        contentStart,
        content,
        compression,
        width,
        height,
        deep,
        sizeDIB,
        palette,
        pixels,
    };

    return bmp;
}