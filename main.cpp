#include <iostream>
#include <cstring>
using namespace std;

class Compiler {
    unsigned int dataBusLength = 256; //initialize the lenth of compiler dynamic memmory
    unsigned int currentPossition = 0;
    int* dataBus;

public:
    Compiler(int dataBusLengthParam);
    void print();

    bool leftStep();
    bool rightStep();
    bool increment();
    bool decrement();
};

Compiler::Compiler(int dataBusLengthParam){
    dataBusLength = dataBusLengthParam;
    dataBus = new int[dataBusLengthParam]; //Create a new dataBus array, whihc will store the data which is set by an input

    for (int i = 0; i < dataBusLengthParam; i++) {
        dataBus[i] = i; //Test fill dynamic array with values from 0 - given length   
    }
}

void Compiler::print()
{
    for (int u = 0; u < dataBusLength; u++) {
        cout << dataBus[u] << " ";
    }
    cout << endl;

}

bool Compiler::leftStep(){ 
    if (currentPossition == 0){
        currentPossition = dataBusLength;
    } else{
        currentPossition = currentPossition - 1;

    }
    return true;
}

bool Compiler::rightStep(){
    if (currentPossition == (dataBusLength-1)){
        currentPossition = 0;
    } else{
        currentPossition = currentPossition+ 1;
    }
    return true;
}

bool Compiler::increment(){
    return true;
}

bool Compiler::decrement(){
    return true;
}
int main() {
    Compiler compiler(256);
    int inputValue;

    while (true) {
        cin >> inputValue;
        
        switch (inputValue){
        case 1:
            compiler.leftStep();
        case 2:
            compiler.rightStep();
        case 3:
            compiler.increment();
        case 4:
            compiler.decrement();
        case 5:

        case 6:

        case 7:

        default:
            break;
        }
    }
}