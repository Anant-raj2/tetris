#pragma once

#include "ansi.h"
#include "cell.h"
#include "piece.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <string_view>
#include <utility>

class Tetris {
  public:
    Tetris() {
        for (size_t i{}; i < m_numRows; ++i) {
            for (size_t j{}; j < m_numColumns; ++j) {
                board[i][j] = Cell{j, (m_numRows - 1) - i};
            }
        }
    }
    void clearScreen();
    void updateState();
    void drawBoard();
    bool shouldQuit();

  private:
    static constexpr int m_numRows{20};
    static constexpr int m_numColumns{10};
    static inline std::array<std::array<Cell, m_numColumns>, m_numRows> board{};
    static constexpr std::pair<size_t, size_t> spawnCoords{m_numColumns / 2 - 2,
                                                           m_numRows};
    Piece m_currPiece{initializePiece(Piece::moving)};
    // Are we already concentrated on a piece

    Cell& getCell(size_t x, size_t y);

    void clearCoords();
    void updateCells();
    bool checkCollision();
    void setPieceState(Piece::State State);
    Piece initializePiece(Piece::State state) {
        return Piece{spawnCoords.first, spawnCoords.second-1, state};
    };
};
