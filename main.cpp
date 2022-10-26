#include <iostream>
#include <cstring>
using namespace std;

class Compiler {
    int dataBusLength = 256; //initialize the lenth of compiler dynamic memmory
    int* dataBus;

public:
    Compiler(int dataBusLengthParam);
    void print();
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


int main() {
    Compiler compiler(256);
    compiler.print();
}