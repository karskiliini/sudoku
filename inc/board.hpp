#ifndef BOARD_HPP
#define BOARD_HPP

#include "coord.hpp"
#include <memory>

class Ninth;
class Cell;

class Board {
public:
    class Private;

    Board();
    Board(const char *filename);
    ~Board();

    Ninth *getNinth(Coord coord);
    Ninth *getNinth(uint32_t x, uint32_t y);
    Cell *at(uint32_t x, uint32_t y);
    void print(bool details = false);

private:
    std::unique_ptr<Private> mPriv;
};

#endif // BOARD_HPP