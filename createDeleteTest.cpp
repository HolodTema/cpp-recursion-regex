#include <iostream>

char ** createArray(int rows, int cols) {
    char ** array = new char*[rows];
    for (int i = 0; i<rows; ++i) {
        array[i] = new char[cols];
    }
    return array;
}

void deleteArray(char **array, int rows, int cols) {
    for (int i = 0; i<rows; ++i) {
        delete [] array[i];
    }
    delete [] array;
}
int main() {
    char ** array;
    try {
        array = createArray(256, 256);
    }
    catch (std::bad_alloc &e) {
        std::cout << "badalloc during array creating\n";
        return 1;
    }

    deleteArray(array, 256, 256);
}