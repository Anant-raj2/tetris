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
    Tetris() { m_board = initializeBoard(); }
    void clearScreen();
    void updateState(Piece::Direction direction);
    void drawBoard();
    bool shouldQuit();
    static Piece::Direction parseInput(char input);

  private:
    static constexpr int m_numRows{20};
    static constexpr int m_numColumns{10};
    using Board = std::array<std::array<Cell, m_numColumns>, m_numRows>;
    static inline int m_score{};
    bool m_quit{false};
    static inline Board m_board;
    static constexpr Piece::Coord m_spawnCoords{(m_numColumns - 1) / 2,
                                                m_numRows - 1};
    Piece m_currPiece{initializePiece()};

    Cell& getCell(const Board& board, int x, int y);

    void clearCoords();
    void updateCells();
    bool checkBelow();
    bool checkCollision(Piece::Direction dir);
    void setPieceState(Piece::State State);

    Piece initializePiece(Piece::State state = Piece::initial);
    Board initializeBoard();
    std::array<Cell, m_numColumns> initializeRow(int rowIdx);

    void updatePieceState(Piece::Direction dir);
    std::vector<Piece::Direction> checkCollisions();
    bool isValidDir(Piece::Position coords);
    bool isComplete(int rowIdx);
    bool destroyCompleteLines();
};
