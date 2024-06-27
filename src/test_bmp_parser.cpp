#include <test_bmp_parser.hpp>

Bmp getBmpMsg () {
    std::vector<char> buffer = readFile("./image_test/16bit_color_small.bmp");
    return bmpParser(buffer);
}

void testBmpPaserContentStart (const Bmp msg) {
    size_t expect = 0xca;
    size_t result = msg.contentStart;
    ensure<size_t>(result, expect, "testBmpPaser contentStart");
}

void testBmpPaser () {
    Bmp msg = getBmpMsg();
    testBmpPaserContentStart(msg);
}