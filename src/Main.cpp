#include <bitset>
#include <chrono>
#include <limits>
#include <thread>
#include <Windows.h>

#include "Galil.h"

#undef max  // undefine Windows max macro

static void demonstration(Galil* galil);
static void userBinaryInput(int* input);
static void userIntInput(int* input);
static void userWait();
static void printInt(int value);
static void printFloat(float value);
static std::chrono::milliseconds timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before);
static bool getBit(int value, int n);
static uint8_t getByte(bool bank, uint16_t value);
static std::bitset<16> itob(uint16_t value);
static uint16_t btoi(std::bitset<16> value);
static float dac(uint8_t value);
static uint16_t adc(float value);
static void di2dvm2motor(Galil* galil, uint8_t value);
static void motorSpeed(Galil* galil);
static void motorPosition(Galil* galil);

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    Galil* galil = new Galil(embf, "192.168.1.120 -d");
    demonstration(galil);
    userWait();
    delete galil;
    delete embf;
    return 0;
}

/**************************************************/
// Write code here for demonstration
static void demonstration(Galil* galil) {
    /*
    using namespace std::chrono_literals;
    auto start = std::chrono::steady_clock::now();
    while (timeDiff(std::chrono::steady_clock::now(), start) < 1h) {

        if (timeDiff(std::chrono::steady_clock::now(), start) == 1s) {
            galil->AnalogOutput(7, 5);
        }
        if (timeDiff(std::chrono::steady_clock::now(), start) == 2s) {
            galil->AnalogOutput(7, 0);
            start = std::chrono::steady_clock::now();
        }

    }
    */
    userWait();
}
/**************************************************/

// Gets a bit pattern from cin
static void userBinaryInput(int* input) {
    std::bitset<16> bits;
    std::cin >> bits;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    *input = btoi(bits);
}

// Gets an int from cin
static void userIntInput(int* input) {
    std::cin >> *input;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Pauses the program until ENTER key press
static void userWait() {
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
}

// Print an integer value after unsigned typecast
static void printInt(int value) {
    std::cout << (unsigned)value << std::endl;
}

// Print a float value
static void printFloat(float value) {
    std::cout << value << std::endl;
}

// Helper function to get the time difference
static std::chrono::milliseconds timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(later - before);
}

// Get nth bit
static bool getBit(int value, int n) {
    return (value >> n) & 1;
}

// Get high or low byte from 16 bit int
static uint8_t getByte(bool bank, uint16_t value) {
    return value & (0x00FF ^ (bank * 0xFFFF));
}

// Convert uint16_t to bitset<16>
static std::bitset<16> itob(uint16_t value) {
    return std::bitset<16>(value);
}

// Convert bitset<16> to uint16_t
static uint16_t btoi(std::bitset<16> value) {
    return value.to_ulong();
}

// Digital to analog converter
static float dac(uint8_t value) {
    return 5.0 / 127.0 * (128.0 - value);
}

// Analog to digital converter
static uint16_t adc(float value) {
    return 128.0 - 127.0 / 5.0 * value;
}

// Write digital input value to digital voltmeter to motor
static void di2dvm2motor(Galil* galil, uint8_t value) {
    galil->DigitalOutput(value);
    float voltage = galil->AnalogInput(0);
    galil->AnalogOutput(7, voltage);
}

// Control the motor speed
static void motorSpeed(Galil* galil) {
    galil->AnalogOutput(7, 0);
    galil->WriteEncoder();
    galil->setSetPoint(10);
    galil->setKd(0.1);
    galil->setKi(0.1);
    galil->setKp(1);
    galil->SpeedControl(true);
}

// Control the motor position
static void motorPosition(Galil* galil) {
    galil->AnalogOutput(7, 0);
    galil->WriteEncoder();
    galil->setSetPoint(10);
    galil->setKd(0.1);
    galil->setKi(0.1);
    galil->setKp(1);
    galil->PositionControl(true);
}
