#include "Galil.h"

#include <bitset>
#include <chrono>
#include <limits>
#include <thread>
#include <Windows.h>

#undef max // undefine Windows max macro

static void demonstration(Galil* galil);
static void userBinaryInput(int* input);
static void userIntInput(int* input);
static void userWait();
static void printInt(int value);
static auto timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before) -> std::chrono::milliseconds;
static bool getBit(int value, int n);
static uint8_t getByte(bool bank, uint16_t value);
static std::bitset<16> itob(uint16_t value);
static uint16_t btoi(std::bitset<16> value);
static float dac(uint8_t value);
static uint16_t adc(float value);
static void route(Galil* galil, uint8_t value);

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    Galil* galil = new Galil(embf, "192.168.1.120 -d");
    //demonstration(galil);
    galil->DigitalOutput(30 << 8U);
    galil->DigitalByteOutput(1, 30);
    printInt(galil->DigitalInput());
    printInt(galil->DigitalByteInput(1));
    userWait();
    delete galil;
    delete embf;
    return 0;
}

/**************************************************/
// Write code here for demonstration
static void demonstration(Galil* galil) {
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

// Helper function to get the time difference
static auto timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before) -> std::chrono::milliseconds {
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
    return -5 / 127 * ((float)value - 128);
}

// Analog to digital converter
static uint16_t adc(float value) {
    return -127 / 5 * value + 128;
}

// Route digital output to analog output 7
static void route(Galil* galil, uint8_t value) {
    galil->DigitalOutput(value);
    float voltage = galil->AnalogInput(0);
    galil->AnalogOutput(7, voltage);
}
