#include "tetris.h"
#include "piece.h"
#include <iostream>
#include <optional>

Cell& Tetris::getCell(size_t x, size_t y) {
    assert(x <= m_numColumns - 1);
    assert(y <= m_numRows - 1);

    size_t cell_x{x};
    size_t cell_y{(m_numRows - 1) - y};
    return board[cell_y][cell_x];
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
    if (checkBelow()) {
        setPieceState(Piece::stationary);
        m_currPiece = initializePiece(Piece::moving);
        updateCells();
        return;
    }
    clearCoords();
    m_currPiece.translate(1, dir);
    updateCells();
}

void Tetris::updateCells() {
    for (auto& cell : m_currPiece.getCells()) {
        Cell& newCell = getCell(cell.getXCoord(), cell.getYCoord());
        newCell = cell;
    }
}

void Tetris::drawBoard() {
    std::cout << Ansi::get(Ansi::greyBg);
    for (size_t i{}; i < m_numColumns * 3 + 2; ++i) {
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
    for (size_t i{}; i < m_numColumns * 3 + 2; ++i) {
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
        if (cell.getYCoord() == spawnCoords.second) {
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
    case 'l':
        return Piece::right;
    default:
        return Piece::invalid;
    }
}
