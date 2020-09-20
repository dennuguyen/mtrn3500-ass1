#include "Galil.h"

static void userWait() {
    std::cout << "Press ENTER to continue..." << std::endl;
    std::cin.ignore();
}

int main(void) {
    EmbeddedFunctions* embf = new EmbeddedFunctions();
    GCStringIn addr = "192.168.172.58";
    Galil galil(embf, addr);
    userWait();
    return 0;
}