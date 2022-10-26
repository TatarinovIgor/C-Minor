#include <iostream>

class Compiler {
    unsigned int dataBusLength = 256; //initialize the lenth of compiler dynamic memmory
    unsigned int currentPosition = 0;
    unsigned int maxValue = 127;
    unsigned int* dataBus;

public:
    explicit Compiler(int dataBusLengthParam);
    void print();

    static unsigned int integerLength(unsigned int numberLengthToFind);

    bool leftStep();
    bool rightStep();
    bool increment();
    bool decrement();
    bool readValue();
    bool writeValue(int valueToWrite);

};

Compiler::Compiler(int dataBusLengthParam){
    dataBusLength = dataBusLengthParam;
    dataBus = new unsigned int[dataBusLengthParam]; //Create a new dataBus array, whihc will store the data which is set by an input

    for (int i = 0; i < dataBusLengthParam; i++) {
        dataBus[i] = 0; //Test fill dynamic array with values from 0 - given length
    }
}

void Compiler::print(){
    unsigned int countCursorPosition, alternativeCountCursorPosition;
    countCursorPosition = 0;
    alternativeCountCursorPosition = 0;
    for (int u = 0; u < dataBusLength; u++) {
        std::cout << dataBus[u] << " ";
        /*
        countCursorPosition = countCursorPosition + integerLength(dataBus[u]) + 1;
        if (dataBus[u] != 0){
            alternativeCountCursorPosition = countCursorPosition;
        }
        */
    }
    std::cout << std::endl;

    for (int u = 0; u < currentPosition; u++){
        countCursorPosition = countCursorPosition + integerLength(dataBus[u]);
    }
    for (int u = 0; u < countCursorPosition+currentPosition; u++){
        std::cout << ".";
    }

    std::cout << "^" << std::endl;
    std::cout << currentPosition << std::endl;

    /* for (int u = 0; u < alternativeCountCursorPosition-2; u++){std::cout << " ";}*/ //Output last filled memory cell

}

unsigned int Compiler::integerLength(unsigned int numberLengthToFind) {
    unsigned int length;
    length = 0;
    do {
        numberLengthToFind /= 10;
        ++length;
    } while (numberLengthToFind != 0);
    return length;
}

bool Compiler::leftStep(){ 
    if (currentPosition == 0){
        currentPosition = dataBusLength;
    } else{
        currentPosition = currentPosition - 1;
    }
    std::cout << "leftStep call" << std::endl;
    return true;
}

bool Compiler::rightStep(){
    if (currentPosition == (dataBusLength-1)){
        currentPosition = 0;
    } else{
        currentPosition = currentPosition + 1;
    }
    std::cout << "rightStep call" << std::endl;
    return true;
}

bool Compiler::increment(){
    std::cout << "increment" << std::endl;
    if (dataBus[currentPosition] == maxValue){
        dataBus[currentPosition] = 0;
    } else {
        dataBus[currentPosition] = dataBus[currentPosition] + 1;
    }
    return true;
}

bool Compiler::decrement(){
    std::cout << "decrement" << std::endl;
    if (dataBus[currentPosition] == 0){
        dataBus[currentPosition] = maxValue;
    } else {
        dataBus[currentPosition] = dataBus[currentPosition] - 1;
    }
    return true;
}

bool Compiler::writeValue(int valueToWrite){
    dataBus[currentPosition] = valueToWrite;
    std::cout << "write_value" << valueToWrite << std::endl;
    return true;
}

bool Compiler::readValue(){
    std::cout << "read_value" << std::endl;
    std::cout << dataBus[currentPosition] << std::endl;

    return true;
}

int main() {
    Compiler compiler(256);
    char inputValue;
    char charToInput;

    while (true) {
        std::cin >> inputValue;
        switch (inputValue) {
            case '1':
                compiler.leftStep();
                continue;
            case '2':
                compiler.rightStep();
                continue;
            case '3':
                compiler.increment();
                continue;
            case '4':
                compiler.decrement();
                continue;
            case '5':
                compiler.readValue();
                continue;
            case '6':
                std::cin >> charToInput;
                compiler.writeValue(int(charToInput));
                continue;
            case '7':
                compiler.print();
                continue;
            default:
                std::cout << "the end" << std::endl;
                break;
        }
        compiler.print();
    }
}
