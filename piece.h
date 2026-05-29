#pragma once

#include "ansi.h"
#include "cell.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>

class Piece {
  public:
    enum Direction {
        down,
        right,
        left
    };

    enum State {
        moving,
        stationary
    };

  private:
    static constexpr size_t m_pieceSize{4};
    std::array<Cell, m_pieceSize> cells;
    State m_state{stationary};
    [[maybe_unused]] bool isColliding{false};
    void translateCell(Cell& cell, Direction dir, size_t magn);
    static constexpr std::string_view m_movingColor {Ansi::get(Ansi::greyBg)};
    static constexpr std::string_view m_stationaryColor {Ansi::get(Ansi::whiteBg)};

  public:
    Piece(size_t x, size_t y, State state);
    void translate(size_t magn, Direction dir);
    static constexpr size_t getSize() { return m_pieceSize; }
    const std::array<Cell, m_pieceSize>& getCells() { return cells; }
    bool hasCell(const Cell& nextCell);
    void setState(State newState);
    State getState() { return m_state; }
};
