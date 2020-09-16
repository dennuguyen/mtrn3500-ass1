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
    switch (code) {
        case G_NO_ERROR:
            std::cerr << G_NO_ERROR_S << std::endl;
            break;
        case G_OPEN_ERROR:
            std::cerr << G_OPEN_ERROR_S << std::endl;
            break;
        case G_BAD_LOST_DATA:
            std::cerr << G_BAD_LOST_DATA_S << std::endl;
            break;
        default:
            break;
    }
}

Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address)
    : Functions(Funcs), g(), ReadBuffer(), ControlParameters(), setPoint() {
    Functions->GVersion(ReadBuffer, sizeof(ReadBuffer));
}

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
