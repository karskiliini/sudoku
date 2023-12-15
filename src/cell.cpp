#include "cell.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::vector;

class Cell::Private {
   public:
    Private(const Coord &coordinate) : coord(coordinate) {
        for (int i = 1; i <= 9; ++i) values.push_back(i);
    }
    vector<uint32_t> values;
    Coord coord;
};

Cell::Cell(const Coord &coordinate) {
    // printf("Cell::Cell x %u y %u\n",coordinate.x, coordinate.y);
    mPriv = std::make_unique<Cell::Private>(coordinate);
}

Cell::~Cell() {}

void Cell::Set(uint32_t value) {
#if 0
    printf("Cell::set x %u y %u : %u\n",mPriv->coord.x, mPriv->coord.y, value);
#endif
    mPriv->values.clear();
    mPriv->values.push_back(value);
}

bool Cell::hasValue(uint32_t value) const {
    auto it = std::find(mPriv->values.begin(), mPriv->values.end(), value);
    return (it != mPriv->values.end());
}

bool Cell::Remove(uint32_t value) {
    auto it = std::find(mPriv->values.begin(), mPriv->values.end(), value);
    if (it != mPriv->values.end()) {
#if 0
        if ((mPriv->coord.x < 1) && (mPriv->coord.y < 1))
            printf("Cell::Remove(x: %u y: %u) Value %u\n", mPriv->coord.x, mPriv->coord.y, value);
#endif
        mPriv->values.erase(it);
        if (mPriv->values.size() == 0) {
            printf("error: Removed the last potential value.");
            throw std::exception();
        }

        return true;
    }

    return false;
}

bool Cell::RemoveAllBut(uint32_t value) {
#if 0
    if ((mPriv->coord.x < 1) && (mPriv->coord.y < 1))
        printf("Cell::RemoveAllBut(x: %u y: %u) Value %u\n", mPriv->coord.x, mPriv->coord.y, value);
#endif

    bool ret = false;
    for (uint32_t i = 0; i < 9; ++i) {
        if (value == i) continue;
        bool ret2 = Remove(i);
        ret = ret || ret2;
#if 0
        if (ret2) {
            printf("Cell::RemoveAllBut(x: %u y: %u) Value %u\n", mPriv->coord.x, mPriv->coord.y, i);
        }
#endif
    }
    return ret;
}

bool Cell::RemoveAllBut(vector<uint32_t> values) {
    //
    bool ret = false;
    for (uint32_t value = 1; value <= 9; ++value) {
        bool skip = false;
        for (const auto &preserve : values) {
            if (value == preserve) {
                skip = true;
                break;
            }
        }
        if (!skip) {
            ret |= Remove(value);
        }
    }
    return ret;
}

int Cell::count() const {
#if 0
    for (auto c : mPriv->values) {
        printf("%u ", c);
    }
    printf("\n");
#endif
    return mPriv->values.size();
}

uint32_t Cell::Value() const {
    if (mPriv->values.size() > 1) return 0;
    return mPriv->values[0];
}

std::vector<uint32_t> Cell::values() const { return mPriv->values; }

Coord Cell::getCoord() const { return mPriv->coord; }

void Cell::Print() const {
    printf("Cell x: %u y: %u \n    ", mPriv->coord.x, mPriv->coord.y);

    for (auto c : mPriv->values) {
        printf("%u ", c);
    }
    printf("\n");
}

void Cell::print(uint32_t row) const {
    const std::string white = "   ";
    std::string empty = ".";

    const char v1 = row * 3 + 1 + 48;
    const char v2 = row * 3 + 2 + 48;
    const char v3 = row * 3 + 3 + 48;

    if (hasValue(row * 3 + 1))
        std::cout << v1;
    else
        std::cout << empty;

    if (hasValue(row * 3 + 2))
        std::cout << v2;
    else
        std::cout << empty;

    if (hasValue(row * 3 + 3))
        std::cout << v3;
    else
        std::cout << empty;

    if (mPriv->coord.x < 8) std::cout << white;
}
