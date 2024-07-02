#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <bmp_utils.hpp>
#include <bmp_parser.hpp>
#include <test_to_number.hpp>
#include <test_bmp_parser.hpp>

int main() {
    // std::vector<char> buffer = readFile("./image_test/16bit_color_small.bmp");

    // bmpParser(buffer);

    // TODO: 迁移到test目录
    testToNumber();
    testBmpParser();

    // sf::RenderWindow window(sf::VideoMode(800, 600), "bmp viewer");

    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     window.clear();

    //     window.display();
    // }
    return 0;
}
