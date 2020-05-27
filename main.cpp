#ifdef __linux__
#define LINUX true
#define WINDOWS false
#endif

#ifdef _WIN32
#define LINUX false
#define WINDOWS true
#endif

#include "GofLife.h"

int main() {
    GofLife gof = GofLife();
    const char FILE_NAME[] = "fight.txt";
    gof.populateWorld(FILE_NAME);
    gof.showWorld();

    const int NUM_GENERATIONS = 100; //set to a smaller number for debugging
    for (int iteration = 0; iteration < NUM_GENERATIONS; iteration++){
        if (WINDOWS)
            system("cmd.exe /c cls"); //Windows only
        else
            system("clear"); //Linux only

        gof.iterateGeneration();
        gof.showWorld();
    }

    if (WINDOWS)
        system("cmd.exe /c PAUSE");

    return 0;
}
