#pragma once

#include "ansi.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>

class Cell {
  public:
    using Coord = std::pair<int, int>;

    Cell() = default;
    Cell(int x, int y, bool isActive = false,
         std::string_view color = Ansi::get(Ansi::inactiveBg))
        : m_coord{x, y}, m_isActive{isActive}, m_color{color} {}

    bool isActive() const { return m_isActive; }

    int getXCoord() const { return m_coord.first; }
    int getYCoord() const { return m_coord.second; }

    void setCoord(const std::pair<int, int>& newCoord) { m_coord = newCoord; }
    void setIsActive(bool isActive) { m_isActive = isActive; }
    void draw() const { std::cout << m_color << m_rep; }
    void setColor(std::string_view color) { m_color = color; }
    std::string_view getColor() const { return m_color; }

  private:
    Coord m_coord;
    bool m_isActive{false};
    static constexpr std::string_view m_rep = "   ";
    std::string_view m_color;
};
