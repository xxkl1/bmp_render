#include <test_to_number.hpp>

void testToNumber1 () {
    std::vector<char> input = {
        0x01,
        0x02,
        0x03,
        0x04,
    };

    size_t expect = 0x04030201;

    size_t result = toNumber(input);

    ensure<size_t>(expect, result, "testToNumber test 1");
}

void testToNumber2 () {
    std::vector<char> input = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x00,
    };

    size_t expect = 0x04030201;

    size_t result = toNumber(input);

    ensure<size_t>(expect, result, "testToNumber test 2");
}

void testToNumber3 () {
    std::vector<char> input = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x00,
        0x05,
    };

    size_t expect = 0x050004030201;

    size_t result = toNumber(input);

    ensure<size_t>(expect, result, "testToNumber test 3");
}

void testToNumber4 () {
    std::vector<char> input = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x01,
        0x01,
        0x00,
        0x05,
    };

    size_t expect = 0x0500010104030201;

    size_t result = toNumber(input);

    ensure<size_t>(expect, result, "testToNumber test 4");
}

void testToNumber5 () {
    std::vector<char> input = {
        0x01,
    };

    size_t expect = 0x01;

    size_t result = toNumber(input);

    ensure<size_t>(expect, result, "testToNumber test 5");
}

void testToNumber () {
    testToNumber1();
    testToNumber2();
    testToNumber3();
    testToNumber4();
    testToNumber5();
}