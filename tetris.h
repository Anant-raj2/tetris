#pragma once

#include "ansi.h"
#include "cell.h"
#include "piece.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

class Tetris {
  public:
    Tetris() {
        for (int i{}; i < m_numRows; ++i) {
            for (int j{}; j < m_numColumns; ++j) {
                board[static_cast<size_t>(i)][static_cast<size_t>(j)] =
                    Cell{j, (m_numRows - 1) - i};
            }
        }
    }
    void clearScreen();
    void updateState(Piece::Direction direction);
    void drawBoard();
    bool shouldQuit();
    static Piece::Direction parseInput(char input);

  private:
    static constexpr int m_numRows{20};
    static constexpr int m_numColumns{10};
    static inline std::array<std::array<Cell, m_numColumns>, m_numRows> board{};
    static constexpr Piece::Coord m_spawnCoords{(m_numColumns - 1) / 2,
                                                m_numRows};
    Piece m_currPiece{initializePiece()};

    Cell& getCell(int x, int y);

    void clearCoords();
    void updateCells();
    bool checkBelow();
    bool checkCollision(Piece::Direction dir);
    void setPieceState(Piece::State State);
    Piece initializePiece(Piece::State state = Piece::initial) {
        return Piece{m_spawnCoords.first, m_spawnCoords.second - 1, state};
    };
    void updatePieceState(Piece::Direction dir);
    std::vector<Piece::Direction> checkCollisions();
    bool isValidDir(Piece::Position coords);
    void destroyCompleteLines();
};
