#include "tetris.h"
#include "ansi.h"
#include "piece.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

Cell& Tetris::getCell(int x, int y) {
    assert(x <= m_numColumns - 1 && x >= 0);
    assert(y <= m_numRows - 1 && x >= 0);

    int cell_x{x};
    int cell_y{(m_numRows - 1) - y};
    return board[static_cast<size_t>(cell_y)][static_cast<size_t>(cell_x)];
}

void Tetris::clearScreen() { std::cout << Ansi::get(Ansi::clear); }

void Tetris::clearCoords() {
    for (const auto& cell : m_currPiece.getCells()) {
        Cell& newCell = getCell(cell.getXCoord(), cell.getYCoord());
        newCell = Cell{cell.getXCoord(), cell.getYCoord()};
    }
}

// TODO: Rework the flow in this
void Tetris::updateState(Piece::Direction dir) {
    if (m_currPiece.getState() == Piece::initial) {
        setPieceState(Piece::moving);
        return;
    }

    Piece::Position newCoords;
    if (dir == Piece::rotate) {
        newCoords = m_currPiece.constructCoords();
    } else {
        newCoords = m_currPiece.constructCoords(dir);
    }

    if (!isValidDir(newCoords)) {
        if (dir == Piece::down) {
            setPieceState(Piece::stationary);
            m_currPiece = initializePiece(Piece::moving);
            updateCells();
        }
        return;
    }

    if (dir == Piece::rotate) {
        m_currPiece.nextRotation();
    }

    clearCoords();
    m_currPiece.applyCoords(newCoords);
    updateCells();
    // destroyCompleteLines();
}

bool Tetris::isValidDir(Piece::Position coords) {
    if (m_currPiece.getState() == Piece::stationary) return false;
    bool isValid{true};
    for (const auto& coord : coords) {
        if (coord.first >= m_numColumns || coord.first < 0) return false;
        if (coord.second >= m_numRows || coord.second < 0) return false;
        const Cell& boardCell = getCell(coord.first, coord.second);
        if (boardCell.isActive() && !m_currPiece.hasCell(boardCell)) {
            isValid = false;
        }
    }
    return isValid;
}

void Tetris::updateCells() {
    for (auto& cell : m_currPiece.getCells()) {
        Cell& newCell = getCell(cell.getXCoord(), cell.getYCoord());
        newCell = cell;
    }
}

void Tetris::drawBoard() {
    std::cout << Ansi::get(Ansi::greyBg);
    for (int i{}; i < m_numColumns * 3 + 2; ++i) {
        std::cout << " ";
    }
    std::cout << '\n';
    for (const auto& row : board) {
        std::cout << Ansi::get(Ansi::greyBg) << " ";
        for (const auto& cell : row) {
            cell.draw();
        }
        std::cout << Ansi::get(Ansi::greyBg) << " " << '\n';
    }
    for (int i{}; i < m_numColumns * 3 + 2; ++i) {
        std::cout << " ";
    }
    std::cout << Ansi::get(Ansi::defaultBg) << "\n\n";
}

void Tetris::setPieceState(Piece::State state) {
    m_currPiece.setState(state);
    updateCells();
}

bool Tetris::checkBelow() {
    if (m_currPiece.getState() == Piece::stationary) return false;
    for (const auto& cell : m_currPiece.getCells()) {
        if (cell.getYCoord() <= 0) return true;
        const Cell& belowCell = getCell(cell.getXCoord(), cell.getYCoord() - 1);
        if (belowCell.isActive() && !m_currPiece.hasCell(belowCell)) {
            return true;
        };
    }
    return false;
}

bool Tetris::shouldQuit() {
    bool quit{false};
    for (const auto& cell : m_currPiece.getCells()) {
        if (cell.getYCoord() == m_spawnCoords.second) {
            quit = true;
        }
    }
    return quit;
}

Piece::Direction Tetris::parseInput(char input) {
    switch (input) {
    case 'h':
        return Piece::left;
    case 'j':
        return Piece::down;
    case 'k':
        return Piece::rotate;
    case 'l':
        return Piece::right;
    default:
        return Piece::invalid;
    }
}
