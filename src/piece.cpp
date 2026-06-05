#include "piece.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <string_view>

Piece::Piece(int x, int y, State state) : m_state{state} {
    assert(m_colors.size() == maxStates);
    for (int i = 0; i < static_cast<int>(m_cells.size()); ++i) {
        int xOffset{
            m_offsets[static_cast<size_t>(Rotation::up)].data()[i].first};
        int yOffset{
            m_offsets[static_cast<size_t>(Rotation::up)].data()[i].second};
        m_cells.data()[i] =
            Cell{x + xOffset, y + yOffset, true, m_colors[initial]};
    }
}

auto Piece::constructCoords(Direction dir) -> Position {
    Position newCoords{};
    int counter{};
    for (const auto& cell : m_cells) {
        Cell::Coord currCoords{cell.getXCoord(), cell.getYCoord()};
        translateCoords(currCoords, dir);
        newCoords.data()[counter] = currCoords;
        ++counter;
    }
    return newCoords;
}

auto Piece::constructCoords() -> Position {
    Position newCoords{};
    int centerX{m_cells[m_centerIdx].getXCoord()};
    int centerY{m_cells[m_centerIdx].getYCoord()};
    for (int i{}; i < static_cast<int>(m_cells.size()); ++i) {
        // if (i == m_centerIdx) continue;
        Coord currCoords{centerX, centerY};
        rotateCoords(
            currCoords,
            m_rotation == Rotation::down
                ? Rotation::up
                : static_cast<Rotation>(static_cast<int>(m_rotation) + 1),
            i);
        newCoords.data()[i] = currCoords;
    }
    return newCoords;
}

void Piece::rotateCoords(Coord& coords, Rotation targetRotation, int cellIdx) {
    size_t offsetIdx{};
    switch (targetRotation) {
    case Rotation::up:
        offsetIdx = static_cast<size_t>(Rotation::up);
        coords.first += m_offsets[offsetIdx].data()[cellIdx].first;
        coords.second += m_offsets[offsetIdx].data()[cellIdx].second;
        break;
    case Rotation::right:
        offsetIdx = static_cast<size_t>(Rotation::right);
        coords.first += m_offsets[offsetIdx].data()[cellIdx].first;
        coords.second += m_offsets[offsetIdx].data()[cellIdx].second;
        break;
    case Rotation::left:
        offsetIdx = static_cast<size_t>(Rotation::left);
        coords.first += m_offsets[offsetIdx].data()[cellIdx].first;
        coords.second += m_offsets[offsetIdx].data()[cellIdx].second;
        break;
    case Rotation::down:
        offsetIdx = static_cast<size_t>(Rotation::down);
        coords.first += m_offsets[offsetIdx].data()[cellIdx].first;
        coords.second += m_offsets[offsetIdx].data()[cellIdx].second;
        break;
    default:
        break;
    }
}

void Piece::applyCoords(const Position& coords) {
    int coordIdx{};
    for (auto& cell : m_cells) {
        cell.setCoord(coords.data()[coordIdx]);
        ++coordIdx;
    }
}

void Piece::translateCoords(Coord& coords, Direction dir) {
    switch (dir) {
    case down:
        --coords.second;
        break;
    case right:
        ++coords.first;
        break;
    case left:
        --coords.first;
        break;
    default:
        break;
    }
}

bool Piece::hasCell(const Cell& nextCell) {
    bool hasCell{false};
    for (const auto& cell : m_cells) {
        if (cell.getXCoord() == nextCell.getXCoord() &&
            cell.getYCoord() == nextCell.getYCoord()) {
            hasCell = true;
        }
    }
    return hasCell;
}

void Piece::setState(State newState) {
    m_state = newState;
    std::string_view newColor{};
    switch (m_state) {
    case initial:
        newColor = m_colors[initial];
        break;
    case moving:
        newColor = m_colors[moving];
        break;
    case transition:
        newColor = m_colors[transition];
        break;
    case stationary:
        newColor = m_colors[stationary];
        break;
    default:
        break;
    }
    for (auto& cell : m_cells) {
        cell.setColor(newColor);
    }
}
