#include "Galil.h"

#include <cstdlib>
#include <sstream>

const char* CmdFmt::Modbus(char m, int n0, int n1, int n2, int n3, char* str) {
    std::stringstream ss;
    ss << "MB" << m << "=" << n0 << "," << n1 << ",";

    if (1 <= n1 && n1 <= 4 || 15 <= n1 && n1 <= 16)
        ss << n2 << "," << n3 << "," << str << ";";
    else if (5 <= n1 && n1 <= 6)
        ss << n2 << "," << n3 << ";";
    else if (n1 == 7)
        ss << str << ";";
    else if (n1 == -1)
        ss << n2 << "," << str << ";";

    return ss.str().c_str();
}

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

Galil::~Galil() { Functions->GClose(g); }

void Galil::DigitalOutput(uint16_t value) {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 6, value, 16, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 6, value, bank, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 5, bit, val, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
}

uint16_t Galil::DigitalInput() {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 2, 0, 16, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
    return ReadBuffer[0];
}

uint8_t Galil::DigitalByteInput(bool bank) {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 2, 0, bank, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
    return ReadBuffer[0];
}

bool Galil::DigitalBitInput(uint8_t bit) {
    Functions->GCommand(g,
                        Commands.Modbus('h', 0, 2, bit, 1, NULL),
                        ReadBuffer,
                        sizeof(ReadBuffer),
                        0);
    return ReadBuffer[0];
}

bool Galil::CheckSuccessfulWrite() {
    return 0;
}

float Galil::AnalogInput(uint8_t channel) {
    return 0.0;
}

void Galil::AnalogOutput(uint8_t channel, double voltage) {
    std::stringstream Command;
    Command << "AO " << (int)channel << "," << voltage << ";";
    std::cout << Command.str().c_str() << std::endl;
    Functions->GCommand(g, Command.str().c_str(), ReadBuffer, 0, NULL);
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
