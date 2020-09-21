#include "Galil.h"

#include <cstdlib>
#include <string>

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address)
    : Functions(Funcs), g(), ReadBuffer(), ControlParameters(), setPoint() {
    check(Functions->GOpen(address, &g));
    std::cout << *this;
}

Galil::~Galil() {
    DigitalOutput(0);
    AnalogOutput(7, 0);
    check(Functions->GClose(g));
}

void Galil::DigitalOutput(uint16_t value) {
    call("OP " + std::to_string(value) + ";");
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {
    call("OP " + std::to_string(value << (bank * 8U)) + ";");
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    call("OB " + std::to_string(bit) + "," + std::to_string(val) + ";");
}

uint16_t Galil::DigitalInput() {
    for (int i = 0; i < 16; i++)
        call("MG @IN[" + std::to_string(i) + "];");
    return (uint16_t)atoi(ReadBuffer);
}

uint8_t Galil::DigitalByteInput(bool bank) {
    for (int i = 0; i < 8; i++)
        call("MG @IN[" + std::to_string(i + bank * 8U) + "];");
    return (uint8_t)atoi(ReadBuffer);
}

bool Galil::DigitalBitInput(uint8_t bit) {
    call("MG @IN[" + std::to_string(bit) + "];");
    return (bool)atoi(ReadBuffer);
}

bool Galil::CheckSuccessfulWrite() {
    return LastCode == G_NO_ERROR;
}

float Galil::AnalogInput(uint8_t channel) {
    call("MG @AN[" + std::to_string(channel) + "];");
    return (float)atof(ReadBuffer);
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
    call("AO " + std::to_string(channel) + "," + std::to_string(voltage) + ";");
}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
    call("AQ " + std::to_string(channel) + "," + std::to_string(range) + ";");
}

void Galil::WriteEncoder() {
    call("WE 0, 0;");
}

int Galil::ReadEncoder() {
    call("RE 0;");
    return atoi(ReadBuffer);
}

void Galil::setSetPoint(int s) { setPoint = s; }
void Galil::setKp(double gain) { ControlParameters[0] = gain; }
void Galil::setKi(double gain) { ControlParameters[1] = gain; }
void Galil::setKd(double gain) { ControlParameters[2] = gain; }

std::ostream& operator<<(std::ostream& output, Galil& galil)
{
    GSize info_len = 100;
    GCStringOut info = new char[info_len];
    galil.check(galil.Functions->GInfo(galil.g, info, info_len));

    GSize ver_len = 100;
    GCStringOut ver = new char[ver_len];
    galil.check(galil.Functions->GVersion(ver, ver_len));

    output << info << std::endl << std::endl;
    output << ver << std::endl << std::endl;

    delete[] info;
    delete[] ver;

    return output;
}

void Galil::call(std::string Command) {
    std::cout << ":" << Command.c_str() << std::endl;
    check(Functions->GCommand(g, Command.c_str(), ReadBuffer, BUFFER_LEN, NULL));
    std::cout << ReadBuffer;
}

void Galil::check(GReturn code) {
    LastCode = code;

    if (code == G_NO_ERROR)
        return;

    std::cerr << "GCLIB_ERROR " << code << ": ";
    switch (code) {
    case G_NO_ERROR:
        std::cerr << G_NO_ERROR_S;
        break;
    case G_GCLIB_ERROR:
        std::cerr << G_GCLIB_ERROR_S;
        break;
    case G_GCLIB_UTILITY_ERROR:
        std::cerr << G_GCLIB_UTILITY_ERROR_S;
        break;
    case G_GCLIB_UTILITY_IP_TAKEN:
        std::cerr << G_GCLIB_UTILITY_IP_TAKEN_S;
        break;
    case G_GCLIB_NON_BLOCKING_READ_EMPTY:
        std::cerr << G_GCLIB_NON_BLOCKING_READ_EMPTY;
        break;
    case G_TIMEOUT:
        std::cerr << G_TIMEOUT_S;
        break;
    case G_OPEN_ERROR:
        std::cerr << G_OPEN_ERROR_S;
        break;
    case G_READ_ERROR:
        std::cerr << G_READ_ERROR_S;
        break;
    case G_WRITE_ERROR:
        std::cerr << G_WRITE_ERROR_S;
        break;
    case G_INVALID_PREPROCESSOR_OPTIONS:
        std::cerr << G_INVALID_PREPROCESSOR_OPTIONS_S;
        break;
    case G_COMMAND_CALLED_WITH_ILLEGAL_COMMAND:
        std::cerr << G_COMMAND_CALLED_WITH_ILLEGAL_COMMAND_S;
        break;
    case G_DATA_RECORD_ERROR:
        std::cerr << G_DATA_RECORD_ERROR_S;
        break;
    case G_UNSUPPORTED_FUNCTION:
        std::cerr << G_UNSUPPORTED_FUNCTION_S;
        break;
    case G_FIRMWARE_LOAD_NOT_SUPPORTED:
        std::cerr << G_FIRMWARE_LOAD_NOT_SUPPORTED_S;
        break;
    case G_ARRAY_NOT_DIMENSIONED:
        std::cerr << G_ARRAY_NOT_DIMENSIONED_S;
        break;
    case G_ILLEGAL_DATA_IN_PROGRAM:
        std::cerr << G_ILLEGAL_DATA_IN_PROGRAM_S;
        break;
    case G_UNABLE_TO_COMPRESS_PROGRAM_TO_FIT:
        std::cerr << G_UNABLE_TO_COMPRESS_PROGRAM_TO_FIT_S;
        break;
    case G_BAD_RESPONSE_QUESTION_MARK:
        std::cerr << G_BAD_RESPONSE_QUESTION_MARK_S;
        break;
    case G_BAD_VALUE_RANGE:
        std::cerr << G_BAD_VALUE_RANGE_S;
        break;
    case G_BAD_FULL_MEMORY:
        std::cerr << G_BAD_FULL_MEMORY_S;
        break;
    case G_BAD_LOST_DATA:
        std::cerr << G_BAD_LOST_DATA_S;
        break;
    case G_BAD_FILE:
        std::cerr << G_BAD_FILE_S;
        break;
    case G_BAD_ADDRESS:
        std::cerr << G_BAD_ADDRESS_S;
        break;
    case G_BAD_FIRMWARE_LOAD:
        std::cerr << G_BAD_FIRMWARE_LOAD_S;
        break;
    case G_GCAPS_OPEN_ERROR:
        std::cerr << G_GCAPS_OPEN_ERROR_S;
        break;
    case G_GCAPS_SUBSCRIPTION_ERROR:
        std::cerr << G_GCAPS_SUBSCRIPTION_ERROR_S;
        break;
    default:
        break;
    }
    std::cerr << std::endl;
}
