#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>


std::vector<char> readFile (const std::string& filePath) {
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile) {
        throw std::invalid_argument("can't open file");
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    return buffer;
}

void getBmpContent (const std::vector<char>& charVector) {

}

void getRenderXyList (const std::vector<char>& charVector) {
}

int main() {
    std::vector<char> buffer = readFile("./image_test/16bit_color_small.bmp");

    sf::RenderWindow window(sf::VideoMode(800, 600), "bmp viewer");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        window.display();
    }
    return 0;
}
