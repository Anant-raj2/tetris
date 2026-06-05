#include "tetris.h"
#include "ansi.h"
#include "piece.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <ranges>
#include <utility>

Cell& Tetris::getCell(const Board&, int x, int y) {
    assert(x <= m_numColumns - 1 && x >= 0);
    assert(y <= m_numRows - 1 && x >= 0);

    int cell_x{x};
    int cell_y{(m_numRows - 1) - y};
    return m_board[static_cast<size_t>(cell_y)][static_cast<size_t>(cell_x)];
}

void Tetris::clearScreen() { std::cout << Ansi::get(Ansi::clear); }

void Tetris::clearCoords() {
    for (const auto& cell : m_currPiece.getCells()) {
        Cell& newCell = getCell(m_board, cell.getXCoord(), cell.getYCoord());
        newCell = Cell{cell.getXCoord(), cell.getYCoord()};
    }
}

bool Tetris::isComplete(int rowIdx) {
    for (const auto& cell : m_board.data()[rowIdx]) {
        if (!cell.isActive() ||
            cell.getColor() != Ansi::get(Ansi::stationaryBg)) {
            return false;
        }
    }
    return true;
}

// returns bool to signify if the board has had any completed lines.
bool Tetris::destroyCompleteLines() {
    Board newBoard{initializeBoard()};
    int counter{m_numRows - 1};
    int cleared{};

    for (const auto& row : std::views::reverse(m_board)) {
        if (isComplete(counter)) {
            ++cleared;
        } else {
            std::array<Cell, m_numColumns> newRow{
                initializeRow(counter + cleared)};
            // Copy the state values of the old row into the new row
            for (size_t i{}; i < m_numColumns; ++i) {
                // If the cell isn't part of the active cell. don't move it
                if (row[i].getColor() != Ansi::get(Ansi::activeBg)) {
                    newRow[i].setColor(row[i].getColor());
                    newRow[i].setIsActive(row[i].isActive());
                }
            }
            newBoard[static_cast<size_t>(counter + cleared)] = newRow;
        }
        --counter;
    }

    for (int i{}; i < cleared; ++i) {
        newBoard[static_cast<size_t>(i)] = initializeRow(i);
    }

    if (cleared > 0) {
        for (const auto& cell : m_currPiece.getCells()) {
            Cell& newBoardCell =
                getCell(newBoard, cell.getXCoord(), cell.getYCoord());
            newBoardCell = cell;
        }
        m_score += cleared;
        m_board = newBoard;
        return true;
    }
    return false;
}

void Tetris::updateState(Piece::Direction dir) {
    if (destroyCompleteLines()) {
        updateCells();
        return;
    }

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
}

bool Tetris::isValidDir(Piece::Position coords) {
    if (m_currPiece.getState() == Piece::stationary) return false;
    bool isValid{true};
    for (const auto& coord : coords) {
        if (coord.first >= m_numColumns || coord.first < 0) return false;
        if (coord.second >= m_numRows || coord.second < 0) return false;
        const Cell& boardCell = getCell(m_board, coord.first, coord.second);
        if (boardCell.isActive() && !m_currPiece.hasCell(boardCell)) {
            isValid = false;
        }
    }
    return isValid;
}

void Tetris::updateCells() {
    for (auto& cell : m_currPiece.getCells()) {
        Cell& newCell = getCell(m_board, cell.getXCoord(), cell.getYCoord());
        newCell = cell;
    }
}

void Tetris::drawBoard() {
    std::cout << Ansi::get(Ansi::activeBg);
    for (int i{}; i < m_numColumns * 3 + 2; ++i) {
        std::cout << " ";
    }
    std::cout << '\n';
    for (const auto& row : m_board) {
        std::cout << Ansi::get(Ansi::activeBg) << " ";
        for (const auto& cell : row) {
            cell.draw();
        }
        std::cout << Ansi::get(Ansi::activeBg) << " " << '\n';
    }
    for (int i{}; i < m_numColumns * 3 + 2; ++i) {
        std::cout << " ";
    }
    std::cout << Ansi::get(Ansi::inactiveBg) << "\n\n";
    std::cout << "Score: " << m_score << '\n';
}

void Tetris::setPieceState(Piece::State state) {
    m_currPiece.setState(state);
    updateCells();
}

bool Tetris::checkBelow() {
    if (m_currPiece.getState() == Piece::stationary) return false;
    for (const auto& cell : m_currPiece.getCells()) {
        if (cell.getYCoord() <= 0) return true;
        const Cell& belowCell =
            getCell(m_board, cell.getXCoord(), cell.getYCoord() - 1);
        if (belowCell.isActive() && !m_currPiece.hasCell(belowCell)) {
            return true;
        };
    }
    return false;
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

bool Tetris::shouldQuit() {
    bool quit{m_quit};
    for (const auto& cell : m_board[0]) {
        if (cell.isActive() && !m_currPiece.hasCell(cell)) {
            m_quit = true;
            quit = true;
        }
    }
    return quit;
}

Piece Tetris::initializePiece(Piece::State state) {
    Piece newPiece{m_spawnCoords.first, m_spawnCoords.second, state};
    for (const auto& cell : newPiece.getCells()) {
        Cell& boardCell = getCell(m_board, cell.getXCoord(), cell.getYCoord());
        if (boardCell.isActive()) {
            m_quit = true;
        }
    }
    return newPiece;
}

auto Tetris::initializeRow(int rowIdx) -> std::array<Cell, m_numColumns> {
    std::array<Cell, m_numColumns> newRow{};
    for (int j{}; j < m_numColumns; ++j) {
        // Indices to cartesion coordinates conversion
        newRow[static_cast<size_t>(j)] = Cell{j, (m_numRows - 1) - rowIdx};
    }
    return newRow;
}

auto Tetris::initializeBoard() -> Board {
    Board newBoard;
    for (int i{}; i < m_numRows; ++i) {
        newBoard[static_cast<size_t>(i)] = initializeRow(i);
    }
    return newBoard;
}
