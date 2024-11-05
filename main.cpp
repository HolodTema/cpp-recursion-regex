#include <iostream>
#include <fstream>
#include <iomanip>

class NoInputFileException : public std::exception {
public:
    const char * what() const noexcept override {
        return "Error: input file not found\n";
    }
};

class EmptyInputFileException : public std::exception {
public:
    const char * what() const noexcept override {
        return "Error: input file is empty\n";
    }
};

class NoMemoryForInputDataException : public std::exception {
public:
    const char * what() const noexcept override {
        return "Error: unable to allocate memory for input data\n";
    }
};

char *createInputLineArray(unsigned int maxLen) {
    try {
        char *inputLine = new char[maxLen];
        return inputLine;
    }
    catch(std::bad_alloc &e) {
        throw NoMemoryForInputDataException();
    }
}

bool isSign(char **str) {
    bool c = *str[0] == '+' || *str[0] == '-';
    if (c) {
        *str += 1;
    }
    return c;
}

bool isDigit(char **str) {
    bool c = *str[0] == '0' ||
        *str[0] == '1' ||
        *str[0] == '2' ||
        *str[0] == '3' ||
        *str[0] == '4' ||
        *str[0] == '5' ||
        *str[0] == '6' ||
        *str[0] == '7' ||
        *str[0] == '8' ||
        *str[0] == '9';
    if (c) {
        *str += 1;
    }
    return c;
}

bool isUnsignedInt(char ** str) {
    bool c = isDigit(str);
    if (c) {
        isUnsignedInt(str);
    }
    return c;
}

bool isOrder(char ** str) {
    if (*str[0] == 'E') {
        ++(*str);
        return isSign(str) && isUnsignedInt(str);
    }
    return false;
}

bool isMantis(char ** str) {
    char * pointerStartState = &(*str[0]);
    bool c1 = isUnsignedInt(str) && *str[0] == '.' && isUnsignedInt(&(++(*str)));
    if (c1) {
        return true;
    }
    *str = pointerStartState;
    return *str[0] == '.' && isUnsignedInt(&(++(*str)));
}

bool isRealNumber(char ** str) {
    return isSign(str) && isMantis(str) && isOrder(str) && *str[0] == '\0';
}

void printOutputTableHeader(int columnFirstSize, int columnSecondSize) {
    std::cout << std::setw(columnFirstSize) << "input line:" << std::setw(columnSecondSize) << "result:" << "\n";
}

void printInputLineColumn(char *line, int columnFirstSize) {
    std::cout << std::setw(columnFirstSize) << line;
}

void printResultColumn(bool result, int columnSecondSize) {
    std::cout << std::setw(columnSecondSize) << result << "\n";
}

void handleInputAndPrintOutputTable(const char *filenameInput, char *inputLine, int maxLineLen, int columnFirstSize, int columnSecondSize) {
    printOutputTableHeader(columnFirstSize, columnSecondSize);

    std::ifstream in = std::ifstream(filenameInput);
    if (in.is_open()) {
        int i = 0;
        bool result = false;
        char ** inputLinePointer = &inputLine;
        while (!in.eof()) {
            in.getline(inputLine, maxLineLen);
            printInputLineColumn(inputLine, columnFirstSize);
            result = isRealNumber(inputLinePointer);
            printResultColumn(result, columnSecondSize);
            ++i;
        }
        in.close();

        if (i == 0) {
            throw EmptyInputFileException();
        }
    }
    else {
        throw NoInputFileException();
    }
}

void deleteInputLineArray(const char *inputLine) {
    delete[] inputLine;
}

int main() {
    const char* FILENAME_INPUT = "input.txt";
    const unsigned int MAX_INPUT_LINE_LEN = 256;
    const int TABLE_COLUMN_FIRST_SIZE = 40;
    const int TABLE_COLUMN_SECOND_SIZE = 10;

    char *inputLine;
    try {
        inputLine = createInputLineArray(MAX_INPUT_LINE_LEN);
    }
    catch (NoMemoryForInputDataException &e) {
        std::cout << e.what();
        return 1;
    }

    try {
        handleInputAndPrintOutputTable(FILENAME_INPUT, inputLine, MAX_INPUT_LINE_LEN, TABLE_COLUMN_FIRST_SIZE, TABLE_COLUMN_SECOND_SIZE);
    }
    catch (NoInputFileException &e) {
        std::cout << e.what();
        deleteInputLineArray(inputLine);
        return 1;
    }
    catch (EmptyInputFileException &e) {
        std::cout << e.what();
        deleteInputLineArray(inputLine);
        return 1;
    }

    return 0;
}
