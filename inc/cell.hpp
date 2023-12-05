#ifndef CELL_HPP
#define CELL_HPP

#include "coord.hpp"
#include <memory>
#include <vector>

class Cell {
public:
    class Private;

    Cell(const Coord &coordinate);
    ~Cell();

    void Set(uint32_t value);
    bool Remove(uint32_t value);
    bool RemoveAllBut(uint32_t value);
    int count() const;
    uint32_t Value() const;
    std::vector<uint32_t> values() const;
    Coord getCoord() const;
    bool hasValue(uint32_t value) const;

    void Print() const;
    void print(uint32_t row) const;

private:
    std::unique_ptr<Private> mPriv;
};

#endif // CELL_HPP
