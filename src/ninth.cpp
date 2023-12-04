#include "ninth.hpp"

#include <vector>
#include "cell.hpp"

using std::vector;

class Ninth::Private {
public:
    Private(const Coord &coordinate) : coord(coordinate) {
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                cells.push_back(std::make_unique<Cell>(Coord(coord.x * 3 + x, coord.y * 3 + y)));
            }
        }
    }

    vector<std::unique_ptr<Cell>> cells;
    Coord coord;
};

Ninth::Ninth(const Coord &coordinate) {
    // printf("Ninth: x: %u y: %u\n", coordinate.x, coordinate.y);
    mPriv = std::make_unique<Private>(coordinate);
}

Ninth::~Ninth() {
}

Cell *Ninth::at(uint32_t x, uint32_t y)
{
    if ((x >= 9) || (y >= 9)) {
        printf("ninth::at nullptr");
        return nullptr;
    }
    return mPriv->cells[(x % 3) + (y % 3) * 3].get();
}

Coord Ninth::getCoord() const
{
    return mPriv->coord;
}

bool Ninth::hasValue(uint32_t value)
{
    for (const auto& c : mPriv->cells)
    {
        if ((c->hasValue(value)) && (c->count() > 1)) return true;
    }
    return false;
}

bool Ninth::hasValueOnRow(uint32_t value, uint32_t y)
{
    for (uint32_t x = 0; x < 3; ++x) {
        if (at(x, y)->hasValue(value)) return true;
    }
    return false;
}

bool Ninth::hasValueOnCol(uint32_t value, uint32_t x)
{
    for (uint32_t y = 0; y < 3; ++y) {
        if (at(x, y)->hasValue(value)) return true;
    }
    return false;
}

vector<uint32_t> Ninth::hasValueOnRows(uint32_t value)
{
    vector<uint32_t> ret;

    for (uint32_t y = 0; y < 3; ++y) {
        for (uint32_t x = 0; x < 3; ++x) {
            if (at(x, y)->hasValue(value)) {
                ret.push_back(y);
                break;
            }
        }
    }
    return ret;
}

std::vector<uint32_t> Ninth::hasValueOnCols(uint32_t value)
{
    vector<uint32_t> ret;

    for (uint32_t x = 0; x < 3; ++x) {
        for (uint32_t y = 0; y < 3; ++y) {
            if (at(x, y)->hasValue(value)) {
                ret.push_back(x);
                break;
            }
        }
    }
    return ret;
}

bool Ninth::RemoveValueFromRows(uint32_t value, std::vector<uint32_t> rows)
{
    bool ret = false;
    for (auto y : rows) {
        for (uint32_t x = 0; x < 3; ++x) {
            auto cell = at(x, y);
            if (cell->count() > 1) {
                ret |= cell->Remove(value);
            }
        }
    }
    return ret;
}

bool Ninth::RemoveValueFromCols(uint32_t value, std::vector<uint32_t> cols)
{
    bool ret = false;
    for (auto x : cols) {
        for (uint32_t y = 0; y < 3; ++y) {
            auto cell = at(x, y);
            if (cell->count() > 1) {
                ret |= cell->Remove(value);
            }
        }
    }
    return ret;
}
