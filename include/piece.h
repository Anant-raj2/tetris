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
    using Coord = std::pair<int, int>;
    using Position = std::array<std::pair<int, int>, m_pieceSize>;
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
    std::array<Cell, m_pieceSize> m_cells;
    State m_state{moving};

    void translateCoords(Coord& coords, Direction dir);
    void rotateCoords(Coord& coords, Rotation targetRotation, int cellIdx);

    static constexpr std::array<std::string_view, maxStates> m_colors{
        Ansi::get(Ansi::activeBg), Ansi::get(Ansi::activeBg),
        Ansi::get(Ansi::activeBg), Ansi::get(Ansi::stationaryBg)};
    Rotation m_rotation{Rotation::up};
    static constexpr int m_centerIdx{1};
    static constexpr std::array<std::array<std::pair<int, int>, m_pieceSize>,
                                m_pieceSize>
        m_offsets{
            Position{Coord{-1, 0}, Coord{0, 0},  Coord{1, 0},Coord{2, 0}                                                             }, //  u
            Position{ Coord{0, 1}, Coord{0, 0}, Coord{0, -1},
                     Coord{0, -2}                                        }, //  r
            Position{Coord{-1, 0}, Coord{0, 0},  Coord{1, 0}, Coord{2, 0}}, //  l
            Position{ Coord{0, 1}, Coord{0, 0}, Coord{0, -1},
                     Coord{0, -2}                                        }, //  d
    };

  public:
    Piece(int x, int y, State state = initial);
    static constexpr int getSize() { return m_pieceSize; }
    const std::array<Cell, m_pieceSize>& getCells() { return m_cells; }
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
