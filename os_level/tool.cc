#include <iostream>
#include <string>
#include <cstdlib>

// Functie om een string te vertalen volgens een Caesar-codering
std::string translate(const std::string &line, int shift) {
    std::string result = "";
    for (char c : line) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift + 26) % 26 + base;  // +26 to handle negative shifts
        }
        result += c;
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
        return -1;
    }

    int shift = std::atoi(argv[1]);
    std::string line;

    while (std::getline(std::cin, line)) {
        std::cout << translate(line, shift) << std::endl;
    }

    return 0;
}