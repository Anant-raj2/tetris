#include "piece.h"
#include "tetris.h"
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <termios.h>
#include <thread>
#include <unistd.h>

struct termios originalTerminal;

void restoreTerminal() { tcsetattr(STDIN_FILENO, TCSANOW, &originalTerminal); }

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &originalTerminal);
    std::atexit(restoreTerminal);

    struct termios* termios_p = &originalTerminal;
    termios_p->c_lflag &=
        (tcflag_t) ~(ICANON | ECHO); // Disable canonical mode and echo
    termios_p->c_cc[VMIN] = 0;       // Read at least 1 byte
    termios_p->c_cc[VTIME] = 0;      // No timeout
    tcsetattr(STDIN_FILENO, TCSANOW, termios_p);
}

Piece::Direction getDirection() {
    char input{};
    long readStatus = read(STDIN_FILENO, &input, 1);
    Piece::Direction dir{Piece::invalid};
    if (readStatus == 1) {
        dir = Tetris::parseInput(input);
    }
    return dir;
}

int main() {
    enableRawMode();

    Tetris game{};
    int updateCounter{};
    while (true) {
        Piece::Direction direction{getDirection()};

        if (direction != Piece::invalid) {
            int flushStatus = tcflush(STDIN_FILENO, TCIOFLUSH);
            assert(flushStatus == 0);
            game.clearScreen();
            game.updateState(direction);
            game.drawBoard();
        }

        if (++updateCounter % 20 == 0) {
            game.clearScreen();
            game.updateState(Piece::down);
            game.drawBoard();
            updateCounter = 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
