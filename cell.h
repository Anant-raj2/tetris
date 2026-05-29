#pragma once

#include "ansi.h"
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>

class Cell {
  public:
    Cell() = default;
    explicit Cell(size_t x, size_t y, bool isActive = false,
                  std::string_view color = Ansi::get(Ansi::defaultBg))
        : m_coord{x, y}, m_isActive{isActive}, m_color{color} {}

    bool isActive() const { return m_isActive; }

    size_t getXCoord() const { return m_coord.first; }
    size_t getYCoord() const { return m_coord.second; }

    void setCoord(const std::pair<size_t, size_t>& newCoord) {
        m_coord = newCoord;
    }
    void setIsActive(bool isActive) { m_isActive = isActive; }
    void draw() const { std::cout << m_color << m_rep; }
    void setColor(std::string_view color) { m_color = color; }

  private:
    std::pair<size_t, size_t> m_coord;
    bool m_isActive{false};
    static constexpr std::string_view m_rep = "   ";
    std::string_view m_color;
};
