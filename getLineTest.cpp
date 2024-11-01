#include <iostream>
#include <fstream>

int main() {
    std::ifstream in("input.txt");
    char *line = new char[10];
    in.getline(line, 10);
    for (int i = 0; i<10; ++i) {
        std::cout << static_cast<int>(line[i]) << "\t";
    }
}