#ifndef COORD_HPP
#define COORD_HPP

#include <stdint.h>

class Coord {
   public:
    Coord(uint32_t ax, uint32_t ay) : x(ax), y(ay) {}
    bool operator==(const Coord &other) {
        return ((x == other.x) && (y == other.y));
    }
    bool operator!=(const Coord &other) {
        return ((x != other.x) || (y != other.y));
    }

    uint32_t x;
    uint32_t y;
};

#endif  // COORD_HPP