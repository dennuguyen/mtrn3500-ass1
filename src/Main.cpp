#include "Galil.h"

#include <Windows.h>

static void userWait() {
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
}

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    GCStringIn addr = "192.168.1.200 -d";
    Galil galil(embf, addr);
    while (1) {
        galil.AnalogOutput(7, 5);
        Sleep(1000);
        galil.AnalogOutput(7, -5);
        Sleep(1000);
    }
    galil.~Galil();
    delete embf;
    userWait();
    return 0;
}