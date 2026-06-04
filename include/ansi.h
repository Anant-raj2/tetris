#pragma once

#include <array>
#include <string_view>

namespace Ansi {
    enum Code {
        clear,
        activeBg,
        inactiveBg,
        stationaryBg,
        maxCodes
    };

    inline constexpr std::array<std::string_view, maxCodes> escapeCodes{
        "\033[2J\033[1;1H", "\u001b[48;5;240m", "\u001b[49m",
        "\u001b[48;5;255m"};

    inline constexpr std::string_view get(Code code) {
        return escapeCodes[code];
    }
}
