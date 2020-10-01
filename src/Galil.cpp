#include "Galil.h"

#include <cstdlib>
#include <string>

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address)
    : Functions(Funcs), g(0), ReadBuffer(""), ControlParameters{0, 0, 0}, setPoint(0), LastCode(G_NO_ERROR) {
    std::cout << std::endl << "====== INITIALISING =====" << std::endl << std::endl;
    check(Functions->GOpen(address, &g)); // Open the connection
    std::cout << *this;                   // Print GInfo and GVersion
    call("RS 0;");                        // Reset the RIO
    call("IQ 65535;");                    // Set positive logic
    std::cout << std::endl << "=========================" << std::endl << std::endl;
}

Galil::~Galil() {
    std::cout << std::endl << "======== CLOSING ========" << std::endl << std::endl;
    call("RS 0;");               // Reset the RIO
    check(Functions->GClose(g)); // Close the connection
    std::cout << std::endl << "=========================" << std::endl << std::endl;
}

void Galil::DigitalOutput(uint16_t value) {
    call("OP " + std::to_string(value & 0x00FF) + "," + std::to_string((value & 0xFF00) >> 8) + ";");
}

void Galil::DigitalByteOutput(bool bank, uint8_t value) {
    call("OP " + std::to_string(value & (!bank * 8U)) + "," + std::to_string(value & (bank * 8U)) + ";");
}

void Galil::DigitalBitOutput(bool val, uint8_t bit) {
    call("OB " + std::to_string(bit) + "," + std::to_string(val) + ";");
}

uint16_t Galil::DigitalInput() {
    uint16_t result = 0;
    for (unsigned int i = 0; i < 16; i++) {
        call("MG @IN[" + std::to_string(i) + "];");
        result |= (std::stoi(ReadBuffer) << i);
    }
    return result;
}

uint8_t Galil::DigitalByteInput(bool bank) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        call("MG @IN[" + std::to_string(i + bank * 8U) + "];");
        result |= (std::stoi(ReadBuffer) << i);
    }
    return result;
}

bool Galil::DigitalBitInput(uint8_t bit) {
    call("MG @IN[" + std::to_string(bit) + "];");
    return (bool)std::stoi(ReadBuffer);
}

bool Galil::CheckSuccessfulWrite() {
    return LastCode == G_NO_ERROR;
}

float Galil::AnalogInput(uint8_t channel) {
    call("MG @AN[" + std::to_string(channel) + "];");
    return std::stof(ReadBuffer);
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
    return std::stoi(ReadBuffer);
}

void Galil::setSetPoint(int s) { setPoint = s; }
void Galil::setKp(double gain) { ControlParameters[0] = gain; }
void Galil::setKi(double gain) { ControlParameters[1] = gain; }
void Galil::setKd(double gain) { ControlParameters[2] = gain; }

std::ostream& operator<<(std::ostream& output, Galil& galil)
{
    galil.check(galil.Functions->GInfo(galil.g, galil.ReadBuffer, BUFFER_LEN));
    output << galil.ReadBuffer << std::endl << std::endl;
    galil.check(galil.Functions->GVersion(galil.ReadBuffer, BUFFER_LEN));
    output << galil.ReadBuffer << std::endl << std::endl;
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
