#include "Galil.h"

#include <Windows.h>

static void userWait() {
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
}

static void demonstration(Galil *galil) {
    for (int i = 0; i < 2; i++) {
        galil->AnalogOutput(7, 5);
        galil->AnalogInput(7);
        Sleep(1000);
        galil->AnalogOutput(7, -5);
        galil->AnalogInput(7);
        Sleep(1000);
    }
}

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    Galil* galil = new Galil(embf, "192.168.1.200 -d");
    demonstration(galil);
    delete galil;
    delete embf;
    userWait();
    return 0;
}
