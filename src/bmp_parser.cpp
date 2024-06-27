#include <bmp_parser.hpp>

void getBmpContentStart (const std::vector<char>& bmpData) {
    std::vector<char> l = getSubVector(bmpData, 0x0a, 0x0a+4);
}

void getBmpContent (const std::vector<char>& bmpData) {

}

void bmpParser (const std::vector<char>& bmpData) {
    // 打印 vec.begin() 所指向的值
    if (!bmpData.empty()) {
        std::cout << "The value at vec.begin() is: " << *bmpData.begin() << std::endl;
    } else {
        std::cout << "The vector is empty." << std::endl;
    }
    std::cout << std::endl;
}