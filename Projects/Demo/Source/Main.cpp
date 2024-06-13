#include "Game.h"

constexpr const char* kAppTitle = "Project0"; 

int main(int argc, char* argv[]) {
    Game instance;
    instance.setWindowTitle(kAppTitle);
    instance.run(argc, argv);
    
    return 0;
}