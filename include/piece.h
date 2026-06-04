#pragma once

#include "ansi.h"
#include "cell.h"
#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

class Piece {
  public:
    static constexpr int m_pieceSize{4};
    using Position = std::array<Cell::Coord, m_pieceSize>;
    enum Direction {
        down,
        right,
        left,
        rotate,
        invalid
    };

    enum State {
        initial,
        moving,
        transition,
        stationary,
        maxStates
    };

    enum class Rotation {
        up,
        right,
        left,
        down
    };

  private:
    Position m_cells;
    State m_state{moving};

    void translateCoords(Cell::Coord& coords, Direction dir);
    void rotateCoords(Cell::Coord& coords, Rotation targetRotation,
                      int cellIdx);

    static constexpr std::array<std::string_view, maxStates> m_colors{
        Ansi::get(Ansi::activeBg), Ansi::get(Ansi::activeBg),
        Ansi::get(Ansi::activeBg), Ansi::get(Ansi::stationaryBg)};
    Rotation m_rotation{Rotation::up};
    static constexpr int m_centerIdx{1};
    static constexpr std::array<std::array<std::pair<int, int>, m_pieceSize>,
                                m_pieceSize>
        m_offsets{
            Position{Cell::Coord{-1, 0}, Cell::Coord{0, 0},  Cell::Coord{1, 0},
                     Cell::Coord{2, 0} }, //  u
            Position{ Cell::Coord{0, 1}, Cell::Coord{0, 0}, Cell::Coord{0, -1},
                     Cell::Coord{0, -2}}, //  r
            Position{Cell::Coord{-1, 0}, Cell::Coord{0, 0},  Cell::Coord{1, 0},
                     Cell::Coord{2, 0} }, //  l
            Position{ Cell::Coord{0, 1}, Cell::Coord{0, 0}, Cell::Coord{0, -1},
                     Cell::Coord{0, -2}}, //  d
    };

  public:
    Piece(int x, int y, State state = initial);
    static constexpr int getSize() { return m_pieceSize; }
    const Position& getCells() { return m_cells; }
    bool hasCell(const Cell& nextCell);
    void setState(State newState);
    State getState() { return m_state; }
    Position constructCoords(Direction dir);
    Position constructCoords();
    void applyCoords(const Position& coords);
    Rotation getRotation() { return m_rotation; }
    void nextRotation() {
        m_rotation =
            (m_rotation == Rotation::down
                 ? Rotation::up
                 : static_cast<Rotation>(static_cast<int>(m_rotation) + 1));
    }
};
