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


//returns amount lines, which were read to inputLines array
int readInputFile(char ** inputLines, const char *filename, unsigned int maxLineLen, unsigned int maxNumberLines) {
    std::ifstream in(filename);
    if (in.is_open()) {
        int i = 0;
        while(!in.eof() && i < maxNumberLines) {
            in.getline(inputLines[i], maxLineLen);
            ++i;
        }
        in.close();
        if (i==0) {
            throw EmptyInputFileException();
        }
        return i;
    }
    else {
        in.close();
        throw NoInputFileException();
    }

}

char ** createInputLinesArray(unsigned int maxLineLen, unsigned int maxNumberLines) {
    try {
        char **inputLines = new char*[maxNumberLines];
        for (int i = 0; i<maxNumberLines; ++i) {
            inputLines[i] = new char[maxLineLen];
        }
        return inputLines;
    }
    catch (std::bad_alloc &e) {
        throw NoMemoryForInputDataException();
    }
}

bool isTerminator(const char *str, int startIndex) {
    return str[startIndex] == '\0';
}

bool isSign(const char *str, int startIndex) {
    return str[startIndex] == '+' ||
           str[startIndex] == '-';
}

bool isDigit(const char *str, int startIndex) {
    return str[startIndex] == '1' ||
        str[startIndex] == '2' ||
        str[startIndex] == '3' ||
        str[startIndex] == '4' ||
        str[startIndex] == '5' ||
        str[startIndex] == '6' ||
        str[startIndex] == '7' ||
        str[startIndex] == '8' ||
        str[startIndex] == '9' ||
        str[startIndex] == '0';
}

//if it is false - it returns 0
//if it is true, it returns the first char index where this expression ends
//relative index
int isUnsignedInteger(const char *str, int startIndex) {
    if (str[startIndex]=='\0') {
        return 0;
    }
    if (isDigit(str, startIndex)) {
        return isUnsignedInteger(str, startIndex+1) + 1;
    }
    else {
        return 0;
    }
}

//relative index
int isOrder(const char *str, int startIndex) {
    int unsignedIntEnds = isUnsignedInteger(str, startIndex + 2);
    if (str[startIndex]=='E' && isSign(str, startIndex+1) && isUnsignedInteger(str, startIndex+2)) {
        return 2 + unsignedIntEnds;
    }
    else {
        return 0;
    }
}

//if it is false - it returns 0
//if it is true, it returns the first char index where this expression ends
//absolute index
int isMantis(const char *str, int startIndex) {
    int indexFirstIntEnds = isUnsignedInteger(str, startIndex);

    if (indexFirstIntEnds == 0) {
        //there is .<unsignedInt> variant
        if (str[startIndex] == '.') {
            int indexSecondIntEnds = isUnsignedInteger(str, startIndex+1);
            if (indexSecondIntEnds == 0) {
                return 0;
            }
            else {
                return startIndex+1+indexSecondIntEnds;
            }
        }
        else {
            return 0;
        }
    }
    else {
        //there is <unsignedInt>.<unsignedInt> variant
        if (str[startIndex+indexFirstIntEnds]=='.') {
            int indexSecondIntEnds = isUnsignedInteger(str, startIndex+indexFirstIntEnds+1);
            if (indexSecondIntEnds == 0) {
                return 0;
            }
            else {
                return startIndex + indexFirstIntEnds + 1 + indexSecondIntEnds;
            }
        }
        else {
            return 0;
        }
    }
}


bool isRealNumber(const char *str, int startIndex) {
    if (isSign(str, startIndex)) {
        int indexMantisEnds = isMantis(str, startIndex+1);
        if (indexMantisEnds != 0) {
            int orderEnds = isOrder(str, indexMantisEnds);
            if (orderEnds==0) {
                return false;
            }
            else {
                return isTerminator(str, indexMantisEnds+orderEnds);
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void deleteInputLinesFromMemory(char **inputLines, unsigned int len) {
    for (int i = 0; i<len; ++i) {
        delete[] inputLines[i];
    }
    delete [] inputLines;
}


int main() {
    const char *FILENAME_INPUT = "input.txt";
    const unsigned int MAX_INPUT_LINE_LEN = 256;
    const unsigned int MAX_INPUT_LINES_NUMBER = 256;
    const unsigned int COLUMN_OUTPUT_FIRST_WIDTH = 30;
    const unsigned int COLUMN_OUTPUT_SECOND_WIDTH = 10;


    char **inputLines;
    try {
        inputLines = createInputLinesArray(MAX_INPUT_LINE_LEN, MAX_INPUT_LINES_NUMBER);
    }
    catch (NoMemoryForInputDataException &e) {
        std::cout << e.what();
        return 1;
    }

    unsigned int inputLinesNumber = 0;
    try {
        inputLinesNumber = readInputFile(inputLines, FILENAME_INPUT, MAX_INPUT_LINE_LEN, MAX_INPUT_LINES_NUMBER);
    }
    catch (NoInputFileException &e) {
        std::cout << e.what();
        deleteInputLinesFromMemory(inputLines, MAX_INPUT_LINES_NUMBER);
        return 1;
    }
    catch (EmptyInputFileException &e) {
        std::cout << e.what();
        deleteInputLinesFromMemory(inputLines, MAX_INPUT_LINES_NUMBER);
        return 1;
    }

    char *line;
    bool result;
    std::cout << std::setw(COLUMN_OUTPUT_FIRST_WIDTH) << "input line:" << std::setw(COLUMN_OUTPUT_SECOND_WIDTH) << "output:" << "\n";
    for(int i = 0; i<inputLinesNumber; ++i) {
        line = inputLines[i];
        result = isRealNumber(line, 0);
        std::cout << std::setw(COLUMN_OUTPUT_FIRST_WIDTH) << line << std::setw(COLUMN_OUTPUT_SECOND_WIDTH) << result << "\n";
    }

    deleteInputLinesFromMemory(inputLines, MAX_INPUT_LINES_NUMBER);
    return 0;
}
