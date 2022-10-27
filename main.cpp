#include <iostream>
#include <fstream>
#include <cmath>

//Riff Chunk
const std::string chunk_id = "RIFF";
const std::string chunk_size ="----";
const std::string format = "WAVE";


const std::string subchunk1_id = "fmt ";
const int subchunk1_size = 16;
const int audio_format = 1;
const int num_channels = 2;
const int sample_rate = 44100;
const int byte_rate = sample_rate * num_channels * (subchunk1_size / 8);
const int block_align = num_channels * (subchunk1_size / 8);
const int bits_per_sample = 16;

// Data subchunk
const std::string subchunk2_id = "data";
const std::string subchunk2_size = "....";

const int duration = 1;
const int max_amplitude = 32760;
const double frequency = 250;

void write_as_bytes(std::ofstream &file, int value, int byte_size){
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}


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
    for (int u = 0; u < countCursorPosition + currentPosition; u++){
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

bool writeAudioFile(int data_to_write[7]){
    std::ofstream wav;
    wav.open("/home/igor/CLionProjects/C-Minor/test.wav", std::ios::binary);
    if (wav.is_open()) {
        wav << chunk_id;
        wav << chunk_size;
        wav << format;
        wav << subchunk1_id;

        write_as_bytes(wav, subchunk1_size, 4);
        write_as_bytes(wav, audio_format, 2);
        write_as_bytes( wav, num_channels, 2);
        write_as_bytes(wav, sample_rate, 4);
        write_as_bytes(wav, byte_rate, 4);
        write_as_bytes(wav, block_align, 2);
        write_as_bytes(wav, bits_per_sample, 2);

        wav << subchunk2_id;
        wav << subchunk2_size;

        int start_audio = wav.tellp();
        for (int index = 0; index < sizeof(data_to_write); index++) {
            for (int i = 0; i < sample_rate * duration; i++) {
                double amplitude = (double) i* index / sample_rate * max_amplitude;
                double value = sin((2 * 3.14 * i * data_to_write[index] * frequency) / sample_rate);

                double channel1 = amplitude * value / 2;
                double channel2 = (max_amplitude - amplitude) * value;

                write_as_bytes(wav, int(channel1), 2);
                write_as_bytes(wav, int(channel2), 2);
            }
        }
        int end_audio = wav.tellp();
        wav.seekp(start_audio - 4);
        write_as_bytes(wav, end_audio-start_audio, 4);
        wav.seekp(4, std::ios::beg);
        write_as_bytes(wav, end_audio - 8, 4);

    }

    wav.close();

    return true;
}

int main() {
    Compiler compiler(256);
    char inputValue;
    char charToInput;
    /*
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
     */

    int values[7];
    values[0] = 1;
    values[1] = 1;
    values[2] = 1;
    values[3] = 1;
    values[4] = 4;
    values[5] = 3;
    values[6] = 2;

    writeAudioFile(values);
}
