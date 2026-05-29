#include "tetris.h"
#include <thread>

int main() {
    Tetris game{};
    int updateCounter{};
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (++updateCounter % 20 == 0) {
            game.clearScreen();
            game.updateState();
            game.drawBoard();
        }
    }
    return 0;
}
