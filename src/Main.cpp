#include "Galil.h"

#include <chrono>
#include <thread>
#include <Windows.h>

static void demonstration(Galil* galil);
static void userWait();
static auto timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before) -> std::chrono::milliseconds;
static uint16_t getNthBit(uint16_t value, int n);
static void printBits(uint16_t value);

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    Galil* galil = new Galil(embf, "192.168.1.120 -d");
    demonstration(galil);
    delete galil;
    delete embf;
    userWait();
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

// Pauses the program until ENTER key press
static void userWait() {
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
}

// Helper function to get the time difference
static auto timeDiff(std::chrono::steady_clock::time_point later, std::chrono::steady_clock::time_point before) -> std::chrono::milliseconds {
    return std::chrono::duration_cast<std::chrono::milliseconds>(later - before);
}

// Get nth bit
static uint16_t getNthBit(uint16_t value, int n) {
    return (value >> n) & 1;
}

// Print bits from MSB to LSB
static void printBits(uint16_t value) {
    for (int i = 15; i >= 0; i--)
        std::cout << ((value >> i) & 1);
    std::cout << std::endl;
}
