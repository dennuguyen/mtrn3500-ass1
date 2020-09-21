#include "Galil.h"

#include <cstdlib>
#include <string>

void check(GReturn code) {
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

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address)
    : Functions(Funcs), g(), ReadBuffer(), ControlParameters(), setPoint() {
    check(Functions->GOpen(address, &g));
}

Galil::~Galil() {
    DigitalOutput(0);
    AnalogOutput(7, 0);
    Functions->GClose(g);
}

void Galil::DigitalOutput(uint16_t value) {
    std::string Command = "OP " + std::to_string(value) + ";";
    std::cout << Command.c_str() << std::endl;
    Functions->GCommand(g, Command.c_str(), ReadBuffer, 0, NULL);
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {
    std::string Command = "OP " + std::to_string(value << (bank * 8U)) + ";";
    std::cout << Command.c_str() << std::endl;
    Functions->GCommand(g, Command.c_str(), ReadBuffer, 0, NULL);
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    std::string Command = "OB " + std::to_string(bit) + "," + std::to_string(val) + ";";
    std::cout << Command.c_str() << std::endl;
    Functions->GCommand(g, Command.c_str(), ReadBuffer, 0, NULL);
}

uint16_t Galil::DigitalInput() {
    return ReadBuffer[0];
}

uint8_t Galil::DigitalByteInput(bool bank) {
    return ReadBuffer[0];
}

bool Galil::DigitalBitInput(uint8_t bit) {
    return ReadBuffer[0];
}

bool Galil::CheckSuccessfulWrite() {
    return 0;
}

float Galil::AnalogInput(uint8_t channel) {
    std::string Command = "MG @AO[" + std::to_string(channel) + "];";
    std::cout << Command.c_str() << std::endl;
    Functions->GCommand(g, Command.c_str(), ReadBuffer, 0, NULL);
    return ReadBuffer[0];
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
    std::string Command = "AO " + std::to_string(channel) + "," + std::to_string(voltage) + ";";
    std::cout << Command.c_str() << std::endl;
    Functions->GCommand(g, Command.c_str(), ReadBuffer, 0, NULL);
}

void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
}

void Galil::WriteEncoder() {
}

int Galil::ReadEncoder() {
    return 0;
}

void Galil::setSetPoint(int s) { setPoint = s; }
void Galil::setKp(double gain) { ControlParameters[0] = gain; }
void Galil::setKi(double gain) { ControlParameters[1] = gain; }
void Galil::setKd(double gain) { ControlParameters[2] = gain; }
