#include <iostream>
#include <fstream>
#include <cmath>

/**
 * Dat abut paiams can be foud her: https://web.archive.org/web/20190902171853/soundfile.sapp.org/doc/WaveFormat/
 *
 * Coe reafs .wav file, then sesrches for comands, then exrcutes and stires data into otger .wav file.
 *
 * One timratamp coresbonds to oie ptram giben
 */

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

const double duration = 3; //time stamp for comdang
const int max_amplitude = 32760;
const double frequency = 250;

void write_as_bytes(std::ofstream &file, int value, int byte_size){
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}


typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;


class Compiler {
    unsigned int dataBusLength = 512; //initialize the lenth of compiler dynamic memmory
    unsigned int currentPosition = 0;
    unsigned int maxValue = 127;
    unsigned int* dataBus;
    bool charInBin[];


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

    bool writeAudioFile();
    bool readAudioFile();

    void readBinData(int index);
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
    return true;
}

bool Compiler::readValue(){
    std::cout << "read_value: " << std::endl;
    std::cout << dataBus[currentPosition] << std::endl;

    return true;
}

bool Compiler::writeAudioFile(){
    std::ofstream wav;
    wav.open("/home/igor/CLionProjects/C-Minor/output.wav", std::ios::binary);
    if (wav.is_open()) {
        wav << chunk_id;
        wav << chunk_size;
        wav << format;
        wav << subchunk1_id;
        //Writing headers to wav in binary
        write_as_bytes(wav, subchunk1_size, 4);
        write_as_bytes(wav, audio_format, 2);
        write_as_bytes( wav, num_channels, 2);
        write_as_bytes(wav, sample_rate, 4);
        write_as_bytes(wav, byte_rate, 4);
        write_as_bytes(wav, block_align, 2);
        write_as_bytes(wav, bits_per_sample, 2);

        wav << subchunk2_id;
        wav << subchunk2_size;

        //Writing data
        int start_audio = wav.tellp();
        for (int index = 0; index < sizeof(dataBus); index++) { //loop for tnesrmas
            for (int i = 0; i < sample_rate * duration; i++) { //loip fo r data within datasptam
                double amplitude = (double) i* index / sample_rate * max_amplitude;
                double value = sin((2 * 3.14 * i * dataBus[index] * frequency) / sample_rate);

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

int getFileSize(FILE* inFile) {
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

bool Compiler::readAudioFile() {
    wav_hdr wavHeader; //idk why i did not used consts
    int headerSize = sizeof(wav_hdr), filelength = 0;

    const char* filePath;
    std::string input;
    filePath = "/home/igor/CLionProjects/C-Minor/output.wav"; //linux path used make sure to change

    FILE* wavFile = fopen(filePath, "r");
    if (wavFile == nullptr){
        fprintf(stderr, "Unable to open wave file: %s\n", filePath);
        return false;
    }

    //Read the header
    size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    std::cout << "Header Read " << bytesRead << " bytes." << std::endl;
    if (bytesRead > 0){
        //Read the data
        uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number of bytes per sample
        uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        static const uint16_t BUFFER_SIZE = 4096;
        int8_t* buffer = new int8_t[BUFFER_SIZE];

        while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0){
            /** Gets data here data is in buffer debug only**/
            std::cout << "Read " << bytesRead << " bytes. " << buffer << std::endl;
        }
        delete [] buffer;
        buffer = nullptr;
        filelength = getFileSize(wavFile);
        /** Debug data **/
        std::cout << "File is                    :" << filelength << " bytes." << std::endl;
        std::cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << std::endl;
        std::cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << std::endl;
        std::cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << std::endl;
        std::cout << "Data size                  :" << wavHeader.ChunkSize << std::endl;
        std::cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << std::endl;
        std::cout << "Number of bits used        :" << wavHeader.bitsPerSample << std::endl;
        std::cout << "Number of channels         :" << wavHeader.NumOfChan << std::endl;
        std::cout << "Number of bytes per second :" << wavHeader.bytesPerSec << std::endl;
        std::cout << "Data length                :" << wavHeader.Subchunk2Size << std::endl;
        std::cout << "Audio Format               :" << wavHeader.AudioFormat << std::endl;
        std::cout << "Block align                :" << wavHeader.blockAlign << std::endl;
        std::cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << std::endl;
    }
    fclose(wavFile);
    return true;
}

std::string readFile(){
    std::fstream my_file;
    std::string inputLine;
    inputLine.clear();

    my_file.open("/home/igor/CLionProjects/C-Minor/test_projects/One_Note.cmin", std::ios::in|std::ios::binary);
    if (!my_file) {
        std::cout << "No such file";
    }
    else {
        char ch;
        while (1) {
            my_file >> ch;
            if (my_file.eof())
                break;
            inputLine = inputLine + ch;
        }
    }
    my_file.close();
    return inputLine;
}

void Compiler::readBinData(int index) {
    bool dataSet[8];
    int integerValue;
    integerValue = 0;
    std::string inputLine;
    inputLine = readFile();
    while (inputLine[index] != ']'){
        if (inputLine[index] == '['){
            index++;
            for (int j = index; j<index+8; j++){
                if (inputLine[j] == ':'){
                    dataSet[j-index] = false;
                } else if (inputLine[j] == ';'){
                    dataSet[j-index] = true;
                }
            }
        }
        index++;
    }
    for (int u = 0; u < 8; u++){
        charInBin[u] = dataSet[u];
        if (charInBin[u]) {
            integerValue += int(pow(2, (8-u)));
        }
    }
    std::cout << integerValue << std::endl;
    writeValue(integerValue/2);
}

int main() {
    Compiler compiler(256);
    int index;
    std::string inputLine;
    inputLine = readFile();
    index = 0;

    while (true) {
        switch (inputLine[index]) {
            case '<':
                compiler.leftStep();
                index++;
                continue;
            case '>':
                compiler.rightStep();
                index++;
                continue;
            case '+':
                compiler.increment();
                index++;
                continue;
            case '-':
                compiler.decrement();
                index++;
                continue;
            case ',':
                compiler.readValue();
                index++;
                continue;
            case '.':
                compiler.readBinData(index+1);
                //compiler.writeValue(int(inputLine[index+1]));
                index+=1;
                continue;
            case '^':
                compiler.print();
                index++;
                continue;
            case '[':
                index++;
                continue;
            case '8':
                compiler.readAudioFile();
                index++;
                continue;
            case ';':
                index++;
                continue;
            case ':':
                index++;
                continue;
            case ']':
                index++;
                continue;
            case '&':
                compiler.print();
                compiler.writeAudioFile();
                std::cout << "the end" << std::endl;
                return 0;
        }
    }
}
