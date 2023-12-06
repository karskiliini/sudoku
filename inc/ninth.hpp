#ifndef NINTH_HPP
#define NINTH_HPP

#include "coord.hpp"

#include <memory>
#include <vector>

class Cell;

class Ninth {
   public:
    class Private;
    Ninth(const Coord &coordinate);
    ~Ninth();

    Cell *at(uint32_t x, uint32_t y);
    Coord getCoord() const;

    bool hasValue(uint32_t value);

    bool hasValueOnRow(uint32_t value, uint32_t y);
    bool hasValueOnCol(uint32_t value, uint32_t x);

    std::vector<uint32_t> hasValueOnRows(uint32_t value);
    std::vector<uint32_t> hasValueOnCols(uint32_t value);

    bool RemoveValueFromRows(uint32_t value, std::vector<uint32_t> rows);
    bool RemoveValueFromCols(uint32_t value, std::vector<uint32_t> cols);

   private:
    std::unique_ptr<Private> mPriv;
};

#endif  // NINTH_HPP
