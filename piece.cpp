#include "piece.h"
#include <cassert>
#include <cstddef>
#include <string_view>
#include <utility>

Piece::Piece(size_t x, size_t y, State state = moving) : m_state{state} {
    assert(m_state == moving || m_state == stationary);
    for (size_t i = 0; i < cells.size(); ++i) {
        cells[i] =
            Cell{x + i, y, true,
                 (m_state == moving ? m_movingColor : m_stationaryColor)};
    }
}

void Piece::translate(size_t magn, Direction dir) {
    if (m_state != moving) return;
    for (auto& cell : cells) {
        translateCell(cell, dir, magn);
    }
}

void Piece::translateCell(Cell& cell, Piece::Direction dir, size_t magn) {
    switch (dir) {
    case Piece::down:
        cell.setCoord(std::pair<size_t, size_t>{cell.getXCoord(),
                                                cell.getYCoord() - magn});
        break;
    case Piece::right:
        cell.setCoord(std::pair<size_t, size_t>{cell.getXCoord() + magn,
                                                cell.getYCoord()});
        break;
    case Piece::left:
        cell.setCoord(std::pair<size_t, size_t>{cell.getXCoord() - magn,
                                                cell.getYCoord()});
        break;
    }
}

bool Piece::hasCell(const Cell& nextCell) {
    bool hasCell{false};
    for (const auto& cell : cells) {
        if (cell.getXCoord() == nextCell.getXCoord() &&
            cell.getYCoord() == nextCell.getYCoord()) {
            hasCell = true;
        }
    }
    return hasCell;
}

void Piece::setState(State newState) {
    assert(newState == moving || newState == stationary);
    m_state = newState;
    std::string_view newColor{};
    switch (m_state) {
    case moving:
        newColor = m_movingColor;
        break;
    case stationary:
        newColor = m_stationaryColor;
        break;
    }
    for (auto& cell : cells) {
        cell.setColor(newColor);
    }
}
