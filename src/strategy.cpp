
#include "strategy.hpp"

#include "board.hpp"
#include "cell.hpp"
#include "ninth.hpp"
#include <assert.h>
#include <cstdio>
#include <deque>
#include <exception>
#include <set>
#include <vector>

using std::vector;

bool clearRow(Board *board, Cell *cell) {
    if (!board) throw std::exception();
    if (!cell) throw std::exception();

    bool ret = false;
    const auto coord = cell->getCoord();
    const uint32_t y = coord.y;
    const uint32_t value = cell->Value();

    for (uint32_t x = 0; x < 9; ++x) {
        if (x != coord.x) {
            auto target = board->at(x, y);
            if (!target) {
                printf("nullptr\n");
                throw std::exception();
            }
            if (target->count() > 1) {
                ret |= target->Remove(value);
            }
        }
    }
    return ret;
}

bool clearCol(Board *board, Cell *cell) {
    if (!board) throw std::exception();
    if (!cell) throw std::exception();

    bool ret = false;
    const auto coord = cell->getCoord();
    const uint32_t skip_y = coord.y;
    const uint32_t x = coord.x;
    const uint32_t value = cell->Value();

    for (uint32_t y = 0; y < 9; ++y) {
        if (y != skip_y) {
            auto target = board->at(x, y);
            if (!target) {
                printf("nullptr\n");
                throw std::exception();
            }
            if (target->count() > 1) {
                ret |= target->Remove(value);
            }
        }
    }
    return ret;
}

bool clearNinth(Board *board, Cell *cell) {
    if (!board) throw std::exception();
    if (!cell) throw std::exception();

    bool ret = false;
    auto ninth = board->getNinth(cell->getCoord());
    auto c = ninth->getCoord();

    for (uint32_t y = c.y * 3; y < c.y * 3 + 3; ++y) {
        for (uint32_t x = c.x * 3; x < c.x * 3 + 3; ++x) {
            if (Coord(x, y) != cell->getCoord()) {
#if 0
                printf("clearninth: x %u y %u - cell %u %u \n", x, y, cell->getCoord().x, cell->getCoord().y);
#endif
                auto target = board->at(x, y);
                if (target->count() > 1) {
                    ret |= target->Remove(cell->Value());
                }
            }
        }
    }

    return ret;
}

bool DuplicatesStrategy::run(Board *board) {
    bool modified = false;
    bool run;
    do {
        run = false;
        for (uint32_t x = 0; x < 9; ++x) {
            for (uint32_t y = 0; y < 9; ++y) {
                auto cell = board->at(x, y);
                if (cell->count() == 1) {
                    // found a cell with only 1 possible value
                    run = clearRow(board, cell) || run;
                    run = clearCol(board, cell) || run;
                    run = clearNinth(board, cell) || run;
                }
            }
        }
        if (run) {
            modified = true;
        }
    } while (run);
    return modified;
}

bool foundOnlyOnceNinth(Board *board, Ninth *ninth, uint32_t value) {
    int found = 0;
    for (uint32_t y = ninth->getCoord().y * 3; y < ninth->getCoord().y * 3 + 3;
         ++y) {
        for (uint32_t x = ninth->getCoord().x * 3;
             x < ninth->getCoord().x * 3 + 3; ++x) {
            auto cell = board->at(x, y);
            if (cell->hasValue(value)) ++found;
#if 0
            printf("x %u y %u \n", x, y);
            if ((y >= 6) && (x <= 2))
                cell->Print();
#endif
        }
    }
    return (found == 1);
}

bool leaveOnlyValueNinth(Board *board, Ninth *ninth, uint32_t value) {
    bool ret = false;
    for (uint32_t y = ninth->getCoord().y * 3; y < ninth->getCoord().y * 3 + 3;
         ++y) {
        for (uint32_t x = ninth->getCoord().x * 3;
             x < ninth->getCoord().x * 3 + 3; ++x) {
            auto cell = board->at(x, y);
            if (cell->hasValue(value)) {
                for (uint32_t i = 1; i <= 9; ++i) {
                    if (i != value) {
                        bool ret = cell->Remove(i) || ret;
                    }
                }
            }
        }
    }
    return ret;
}

bool foundOnlyOnceRow(Board *board, uint32_t y, uint32_t value) {
    int found = 0;
    for (uint32_t x = 0; x < 9; ++x) {
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) ++found;
    }

    return (found == 1);
}

bool foundOnlyOnceCol(Board *board, uint32_t x, uint32_t value) {
    int found = 0;
    for (uint32_t y = 0; y < 9; ++y) {
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) ++found;
    }

    return (found == 1);
}

bool leaveOnlyValueRow(Board *board, uint32_t y, uint32_t value) {
    bool ret = false;
    for (uint32_t x = 0; x < 9; ++x) {
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) {
            for (uint32_t i = 1; i <= 9; ++i) {
                if (i != value) ret = cell->Remove(i) || ret;
            }
        }
    }
    return ret;
}

bool leaveOnlyValueCol(Board *board, uint32_t x, uint32_t value) {
    bool ret = false;
    for (uint32_t y = 0; y < 9; ++y) {
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) {
            for (uint32_t i = 1; i <= 9; ++i) {
                if (i != value) ret = cell->Remove(i) || ret;
            }
        }
    }
    return ret;
}

bool OnlyOneStrategy::run(Board *board) {
    bool modified = false;
    bool run;
    do {
        run = false;
        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check) {
            // check each ninth and check for only one value there
            for (uint32_t ny = 0; ny < 3; ++ny) {
                for (uint32_t nx = 0; nx < 3; ++nx) {
                    auto ninth = board->getNinth(nx * 3, ny * 3);
                    if (foundOnlyOnceNinth(board, ninth, check)) {
#if 0
                        if ((ny == 2) && (nx == 0))
                        {
                            printf("ninth: found only once: %u. ninth x %u y%u\n", check, ninth->getCoord().x, ninth->getCoord().y);
                        }
#endif
                        run = leaveOnlyValueNinth(board, ninth, check);
                    }
                }
            }

            // do the same for lines
            for (uint32_t i = 0; i < 9; ++i) {
                if (foundOnlyOnceRow(board, i, check)) {
                    run = leaveOnlyValueRow(board, i, check) || run;
                }

                if (foundOnlyOnceCol(board, i, check)) {
                    run = leaveOnlyValueCol(board, i, check) || run;
                }
            }
        }
        if (run) {
            modified = true;
        }
    } while (run);

    return modified;
}

bool foundOnlyOnRow(Board *board, Ninth *ninth, uint32_t y, uint32_t value) {
    // verify that the value is found on the row
    bool valueFound = false;
    for (uint32_t x = 0; x < 9; ++x) {
        if (ninth != board->getNinth(x, y)) continue;
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) valueFound = true;
    }

    if (!valueFound) return false;

    // now let's go through other rows and see if it's found
    valueFound = false;
    for (uint32_t x = 0; x < 9; ++x) {
        for (uint32_t y2 = 0; y2 < 9; ++y2) {
            if (ninth != board->getNinth(x, y2)) continue;
            if (y2 == y) continue;

            auto cell = board->at(x, y2);
            if (cell->hasValue(value)) return false;
        }
    }
    return true;
}

bool foundOnlyOnCol(Board *board, Ninth *ninth, uint32_t x, uint32_t value) {
    // verify that the value is found on the row
    bool valueFound = false;
    for (uint32_t y = 0; y < 9; ++y) {
        if (ninth != board->getNinth(x, y)) continue;
        auto cell = board->at(x, y);
        if (cell->hasValue(value)) valueFound = true;
    }

    if (!valueFound) return false;

    // now let's go through other rows and see if it's found
    valueFound = false;
    for (uint32_t y = 0; y < 9; ++y) {
        for (uint32_t x2 = 0; x2 < 9; ++x2) {
            if (ninth != board->getNinth(x2, y)) continue;
            if (x2 == x) continue;

            auto cell = board->at(x2, y);
            if (cell->hasValue(value)) return false;
        }
    }
    return true;
}

bool OnlyOnALineStrategy::run(Board *board) {
    bool modified = false;
    bool run;
    do {
        run = false;
        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check) {
            // check each ninth and check for only one value there
            for (uint32_t ny = 0; ny < 3; ++ny) {
                for (uint32_t nx = 0; nx < 3; ++nx) {
                    auto ninth = board->getNinth(nx * 3, ny * 3);

                    for (uint32_t y = 0; y < 9; ++y) {
                        if (foundOnlyOnRow(board, ninth, y, check)) {
                            for (uint32_t x = 0; x < 9; ++x) {
                                auto ninthToClear = board->getNinth(x, y);
                                if (ninthToClear == ninth) continue;

                                auto cell = board->at(x, y);
                                run = cell->Remove(check) || run;
#if 0
                                printf("value %u Found only on row y: %u \n", check, y);
#endif
                            }
                        }
                    }

                    for (uint32_t x = 0; x < 9; ++x) {
                        if (foundOnlyOnCol(board, ninth, x, check)) {
                            for (uint32_t y = 0; y < 9; ++y) {
                                auto ninthToClear = board->getNinth(x, y);
                                if (ninthToClear->getCoord() ==
                                    ninth->getCoord())
                                    continue;

                                auto cell = board->at(x, y);
                                run = cell->Remove(check) || run;
                            }
                        }
                    }
                }
            }

            // do the same for lines
            for (uint32_t i = 0; i < 9; ++i) {
                if (foundOnlyOnceRow(board, i, check)) {
                    run = leaveOnlyValueRow(board, i, check) || run;
                }

                if (foundOnlyOnceCol(board, i, check)) {
                    run = leaveOnlyValueCol(board, i, check) || run;
                }
            }
        }
        if (run) {
            modified = true;
        }
    } while (run);

    return modified;
}

bool DoubleLinesStrategy::run(Board *board) {
    bool modified = false;
    bool run;
    do {
        run = false;

        vector<Ninth *> ninthLine;

        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check) {
            for (uint32_t ny = 0; ny < 9; ny += 3) {
                ninthLine.clear();

                for (uint32_t nx = 0; nx < 9; nx += 3) {
                    ninthLine.push_back(board->getNinth(Coord(nx, ny)));
                }

                for (uint32_t it1 = 0; it1 < ninthLine.size() - 1; ++it1) {
                    for (uint32_t it2 = it1 + 1; it2 < ninthLine.size();
                         ++it2) {
                        for (uint32_t y = 0; y < 3; ++y) {
                            auto rows1 = ninthLine[it1]->hasValueOnRows(check);
                            auto rows2 = ninthLine[it2]->hasValueOnRows(check);

                            if ((rows1.size() == 2) && (rows1 == rows2)) {
                                // the third ninth cannot have this value on
                                // these two rows
                                Ninth *n;
                                if (it1 == 1) {
                                    // first ninth is the one
                                    n = ninthLine[0];
                                } else if (it2 == 2) {
                                    // second ninth is the one
                                    n = ninthLine[1];
                                } else {
                                    // third ninth is the one
                                    n = ninthLine[2];
                                }
                                if (!n) throw std::exception();

                                // printf("removeValueFromRows: %u %u value %u
                                // \n", rows1[0], rows1[1], check);
                                n->RemoveValueFromRows(check, rows1);
                            }
                        }
                    }
                }
            }

            for (uint32_t nx = 0; nx < 9; nx += 3) {
                ninthLine.clear();

                for (uint32_t ny = 0; ny < 9; ny += 3) {
                    ninthLine.push_back(board->getNinth(Coord(nx, ny)));
                }

                for (uint32_t it1 = 0; it1 < ninthLine.size() - 1; ++it1) {
                    for (uint32_t it2 = it1 + 1; it2 < ninthLine.size();
                         ++it2) {
                        for (uint32_t x = 0; x < 3; ++x) {
                            auto cols1 = ninthLine[it1]->hasValueOnCols(check);
                            auto cols2 = ninthLine[it2]->hasValueOnCols(check);

                            if ((cols1.size() == 2) && (cols1 == cols2)) {
                                // the third ninth cannot have this value on
                                // these two rows
                                Ninth *n;
                                if (it1 == 1) {
                                    // first ninth is the one
                                    n = ninthLine[0];
                                } else if (it2 == 2) {
                                    // second ninth is the one
                                    n = ninthLine[1];
                                } else {
                                    // third ninth is the one
                                    n = ninthLine[2];
                                }
                                if (!n) throw std::exception();

                                if (n->hasValue(check)) {
#if 0
                                    if (check == 3)
                                        printf("RemoveValueFromCols: nx: %u %u %u value %u \n", nx, cols1[0], cols1[1], check);
#endif
                                    n->RemoveValueFromCols(check, cols1);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (run) {
            modified = true;
        }
    } while (run);

    return modified;
}

struct DoublesOnLine {
    bool found = false;
    uint32_t index1;
    uint32_t index2;
    uint32_t value;
    uint32_t row_or_col;
};

struct ValuesOnLine;
ValuesOnLine findValuesOnLine(const vector<Cell *> &line, uint32_t value1,
                              uint32_t value2, const uint32_t row_or_col);

// A "double": the value is possible in exactly two unsolved cells of the line
// and is not already solved on the line. Same rules as findValuesOnLine with
// one value, so it delegates to it.
DoublesOnLine findDoubles(const vector<Cell *> &line, uint32_t value,
                          const uint32_t row_or_col);

bool BoxLinesStrategy::run(Board *board) {
    constexpr uint32_t COUNT = 2;

    bool modified = false;
    bool run;
    do {
        run = false;

        {
            for (uint32_t check = 1; check <= 9; ++check) {
                DoublesOnLine found1;
                DoublesOnLine found2;

                for (uint32_t x = 0; x < (9 - (COUNT - 1)); ++x) {
                    auto col1 = board->getCol(x);
                    // printf("check x %u \n", x);

                    // look for double numbers
                    found1 = findDoubles(col1, check, x);
                    if (found1.found) {
                        for (uint32_t x2 = x + 1; x2 < (9 - (COUNT - 2));
                             ++x2) {
                            auto col2 = board->getCol(x2);
                            found2 = findDoubles(col2, check, x2);
                            if (found2.found &&
                                (found1.index1 == found2.index1) &&
                                (found1.index2 == found2.index2)) {
                                assert(found1.value == found2.value);
                                assert(found1.value == check);
#if 0
                printf("value: %u x: %u index1 %u index2 %u found2: %u index1 "
                       "%u index2 %u x: %u \n",
                       found1.value, found1.row_or_col, found1.index1,
                       found1.index2, found2.value, found1.index1,
                       found1.index2, found2.row_or_col);
#endif
                                if (found1.found && found2.found) {
                                    for (uint32_t x = 0; x < 9; ++x) {
                                        if (x != found1.row_or_col &&
                                            x != found2.row_or_col) {
                                            assert(found1.row_or_col !=
                                                   found2.row_or_col);
                                            assert(found1.index1 ==
                                                   found2.index1);
                                            assert(found1.index2 ==
                                                   found2.index2);
                                            run |= board->at(x, found1.index1)
                                                       ->Remove(found1.value);
                                            run |= board->at(x, found1.index2)
                                                       ->Remove(found1.value);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        {
            for (uint32_t check = 1; check <= 9; ++check) {
                DoublesOnLine found1;
                DoublesOnLine found2;

                for (uint32_t y = 0; y < (9 - (COUNT - 1)); ++y) {
                    auto row1 = board->getRow(y);

                    // look for double numbers
                    found1 = findDoubles(row1, check, y);
                    if (found1.found) {
                        for (uint32_t y2 = y + 1; y2 < (9 - (COUNT - 2));
                             ++y2) {
                            auto row2 = board->getRow(y2);
                            found2 = findDoubles(row2, check, y2);
                            if (found2.found &&
                                (found1.index1 == found2.index1) &&
                                (found1.index2 == found2.index2)) {
                                assert(found1.value == found2.value);
                                assert(found1.value == check);
#if 0
                printf("value: %u y1: %u, y2 %u index1 %u index2 %u index1 "
                       "%u index2 %u\n",
                       found1.value, found1.row_or_col, found2.row_or_col,
                       found1.index1, found1.index2, found1.index1,
                       found1.index2);
#endif
                                if (found1.found && found2.found) {
                                    for (uint32_t y = 0; y < 9; ++y) {
                                        if (y != found1.row_or_col &&
                                            y != found2.row_or_col) {
                                            assert(found1.row_or_col !=
                                                   found2.row_or_col);
                                            assert(found1.index1 ==
                                                   found2.index1);
                                            assert(found1.index2 ==
                                                   found2.index2);
                                            run |= board->at(found1.index1, y)
                                                       ->Remove(found1.value);
                                            run |= board->at(found1.index2, y)
                                                       ->Remove(found1.value);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        modified |= run;
    } while (run);

    return modified;
}

struct ValuesOnLine {
    bool found = false;
    uint32_t index1;
    uint32_t value1;
    uint32_t index2;
    uint32_t value2;
    uint32_t row_or_col;
};

ValuesOnLine findValuesOnLine(const vector<Cell *> &line, uint32_t value1,
                              uint32_t value2, const uint32_t row_or_col) {
    // A "hidden pair" on the line: value1 and value2 are both still open on
    // the line and together they are possible in exactly two cells. Every
    // cell of the line has to be examined, including the ones between the
    // two candidate cells, otherwise a value that is also possible elsewhere
    // is mistaken for a pair.
    ValuesOnLine ret;
    vector<uint32_t> positions;
    bool foundValue1 = false;
    bool foundValue2 = false;

    for (uint32_t index = 0; index < line.size(); ++index) {
        const bool has1 = line[index]->hasValue(value1);
        const bool has2 = line[index]->hasValue(value2);
        if (!has1 && !has2) continue;

        // the value is already solved on this line, it cannot form a pair
        if (line[index]->count() == 1) return ret;

        positions.push_back(index);
        foundValue1 |= has1;
        foundValue2 |= has2;
    }

    if ((positions.size() != 2) || !foundValue1 || !foundValue2) return ret;

    ret.found = true;
    ret.index1 = positions[0];
    ret.index2 = positions[1];
    ret.value1 = value1;
    ret.value2 = value2;
    ret.row_or_col = row_or_col;
#if 0
    printf(
        "findValuesOnLine: found: true, index1: %u index2: %u "
        "value1 %u, value2 %u "
        "row_or_col %u\n",
        ret.index1, ret.index2, value1, value2, row_or_col);
#endif
    return ret;
}

DoublesOnLine findDoubles(const vector<Cell *> &line, uint32_t value,
                          const uint32_t row_or_col) {
    DoublesOnLine ret;
    const ValuesOnLine v = findValuesOnLine(line, value, value, row_or_col);
    ret.found = v.found;
    if (v.found) {
        ret.index1 = v.index1;
        ret.index2 = v.index2;
        ret.value = value;
        ret.row_or_col = row_or_col;
    }
    return ret;
}

bool BoxLinesTwoValuesStrategy::run(Board *board) {
    constexpr uint32_t COUNT = 2;

    bool modified = false;
    bool run;
    do {
        run = false;

        for (uint32_t check1 = 1; check1 <= 9; ++check1) {
            for (uint32_t check2 = 1; check2 <= 9; ++check2) {
                ValuesOnLine found1;
                ValuesOnLine found2;

                for (uint32_t x = 0; x < (9 - (COUNT - 1)); ++x) {
                    auto col1 = board->getCol(x);

                    // look for double numbers
                    found1 = findValuesOnLine(col1, check1, check2, x);
                    if (found1.found) {
                        for (uint32_t x2 = x + 1; x2 < (9 - (COUNT - 2));
                             ++x2) {
                            auto col2 = board->getCol(x2);
                            found2 = findValuesOnLine(col2, check1, check2, x2);
                            if (found2.found &&
                                (found1.index1 == found2.index1) &&
                                (found1.index2 == found2.index2)) {
#if 0
                                printf(
                                    "x: %u value1: %u index1 %u value2: %u "
                                    "index2 %u x2: %u "
                                    "value1: %u index1 "
                                    "%u value2: %u index2 %u \n",
                                    found1.row_or_col, found1.value1,
                                    found1.index1, found1.value2, found1.index2,
                                    found2.row_or_col, found2.value1,
                                    found1.index1, found2.value2,
                                    found1.index2);
#endif
                                if (found1.found && found2.found) {
                                    for (uint32_t x3 = 0; x3 < 9; ++x3) {
                                        if (x3 != found1.row_or_col &&
                                            x3 != found2.row_or_col) {
                                            assert(found1.row_or_col !=
                                                   found2.row_or_col);
                                            assert(found1.index1 ==
                                                   found2.index1);
                                            assert(found1.index2 ==
                                                   found2.index2);
                                            run |= board->at(x3, found1.index1)
                                                       ->Remove(found1.value1);
                                            run |= board->at(x3, found1.index2)
                                                       ->Remove(found1.value1);
                                            run |= board->at(x3, found1.index1)
                                                       ->Remove(found1.value2);
                                            run |= board->at(x3, found1.index2)
                                                       ->Remove(found1.value2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (uint32_t check1 = 1; check1 <= 9; ++check1) {
            for (uint32_t check2 = 1; check2 <= 9; ++check2) {
                ValuesOnLine found1;
                ValuesOnLine found2;

                for (uint32_t y = 0; y < (9 - (COUNT - 1)); ++y) {
                    auto row1 = board->getRow(y);

                    // look for double numbers
                    found1 = findValuesOnLine(row1, check1, check2, y);
                    if (found1.found) {
                        for (uint32_t y2 = y + 1; y2 < (9 - (COUNT - 2));
                             ++y2) {
                            auto row2 = board->getRow(y2);
                            found2 = findValuesOnLine(row2, check1, check2, y2);
                            if (found2.found &&
                                (found1.index1 == found2.index1) &&
                                (found1.index2 == found2.index2)) {
#if 0
                                printf(
                                    "x: %u value1: %u index1 %u value2: %u "
                                    "index2 %u x2: %u "
                                    "value1: %u index1 "
                                    "%u value2: %u index2 %u \n",
                                    found1.row_or_col, found1.value1,
                                    found1.index1, found1.value2, found1.index2,
                                    found2.row_or_col, found2.value1,
                                    found1.index1, found2.value2,
                                    found1.index2);
#endif
                                if (found1.found && found2.found) {
                                    for (uint32_t y3 = 0; y3 < 9; ++y3) {
                                        if (y3 != found1.row_or_col &&
                                            y3 != found2.row_or_col) {
                                            assert(found1.row_or_col !=
                                                   found2.row_or_col);
                                            assert(found1.index1 ==
                                                   found2.index1);
                                            assert(found1.index2 ==
                                                   found2.index2);
                                            run |= board->at(found1.index1, y3)
                                                       ->Remove(found1.value1);
                                            run |= board->at(found1.index2, y3)
                                                       ->Remove(found1.value1);
                                            run |= board->at(found1.index1, y3)
                                                       ->Remove(found1.value2);
                                            run |= board->at(found1.index2, y3)
                                                       ->Remove(found1.value2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        modified |= run;

    } while (run);

    return modified;
}

bool findDuplicatesInNinth(Ninth *n, uint32_t check, uint32_t check2,
                           uint32_t x, uint32_t y) {
    if (n->at(x, y)->hasValue(check) && n->at(x, y)->hasValue(check2)) {
        return true;
    }
    return false;
}

bool DoubleNinthStrategy::run(Board *board) {
    bool modified = false;
    bool run;
    do {
        run = false;

        for (uint32_t nx = 0; nx < 9; nx += 3) {
            for (uint32_t ny = 0; ny < 9; ny += 3) {
                auto n = board->getNinth({nx, ny});
                // check for value 'check' and 'check2' in each cell
                for (uint32_t check = 1; check <= 8; ++check) {
                    for (uint32_t check2 = check + 1; check2 <= 9; ++check2) {
                        bool found1 = false;
                        bool found2 = false;
                        Coord coord1{0, 0};
                        Coord coord2{0, 0};
                        bool fail = false;

                        for (uint32_t x = 0; x < 3 && !fail; ++x) {
                            for (uint32_t y = 0; y < 3 && !fail; ++y) {
                                if (findDuplicatesInNinth(n, check, check2, x,
                                                          y)) {
                                    if (!found1) {
                                        found1 = true;
                                        coord1 = {x, y};
                                    } else if (!found2) {
                                        found2 = true;
                                        coord2 = {x, y};
                                    } else {
                                        // failed, continue with next values
                                        fail = true;
                                    }
                                } else if (n->at(x, y)->hasValue(check) ||
                                           n->at(x, y)->hasValue(check2)) {
                                    fail = true;
                                }
                            }
                        }
                        // found suitable candidates
                        if (!fail && found1 && found2) {
                            n->at(coord1)->RemoveAllBut({check, check2});
#if 0
                            board->print(true);
                            board->print();
#endif
                            n->at(coord2)->RemoveAllBut({check, check2});
                        }
                    }
                }
            }
        }
        modified |= run;
    } while (run);
    return modified;
}

// ---------------------------------------------------------------------------
// Chain strategies
// ---------------------------------------------------------------------------

namespace {

constexpr uint32_t kCells = 81;
constexpr uint32_t kNodes = kCells * 9;

inline uint32_t cellIndex(uint32_t x, uint32_t y) { return y * 9 + x; }
inline uint32_t cellX(uint32_t cell) { return cell % 9; }
inline uint32_t cellY(uint32_t cell) { return cell / 9; }
inline uint32_t nodeId(uint32_t cell, uint32_t digit) {
    return cell * 9 + (digit - 1);
}
inline uint32_t nodeCell(uint32_t node) { return node / 9; }
inline uint32_t nodeDigit(uint32_t node) { return node % 9 + 1; }

// true when two different cells share a row, a column or a ninth
bool seesEachOther(uint32_t a, uint32_t b) {
    if (a == b) return false;
    const uint32_t ax = cellX(a), ay = cellY(a);
    const uint32_t bx = cellX(b), by = cellY(b);
    return (ax == bx) || (ay == by) ||
           ((ax / 3 == bx / 3) && (ay / 3 == by / 3));
}

struct ChainLinks {
    bool bivalueStrong;    // two candidates of a bivalue cell
    bool conjugateStrong;  // the only two places for a digit in a unit
    bool cellWeak;         // two different candidates of one cell
    bool unitWeak;         // same digit in two cells of one unit
};

// Implication graph over the candidates of the current board state.
//   strong[n]: nodes m with "n false => m true"
//   weak[n]:   nodes m with "n true  => m false"
// A strong link is always usable as a weak link, so strong[n] is a subset of
// weak[n].
class ChainGraph {
   public:
    ChainGraph(Board *board, const ChainLinks &links) : mBoard(board) {
        present.assign(kNodes, false);
        strong.assign(kNodes, {});
        weak.assign(kNodes, {});

        // A candidate counts only if its cell is unsolved and no cell that
        // sees it is already solved with the same digit. This ignores stale
        // candidates that DuplicatesStrategy has not removed yet, which would
        // otherwise produce false strong links.
        for (uint32_t cell = 0; cell < kCells; ++cell) {
            Cell *c = board->at(cellX(cell), cellY(cell));
            if (c->count() == 1) continue;
            for (uint32_t digit = 1; digit <= 9; ++digit) {
                if (!c->hasValue(digit)) continue;
                bool solvedNearby = false;
                for (uint32_t other = 0; other < kCells && !solvedNearby;
                     ++other) {
                    if (!seesEachOther(cell, other)) continue;
                    Cell *o = board->at(cellX(other), cellY(other));
                    solvedNearby = (o->count() == 1) && (o->Value() == digit);
                }
                present[nodeId(cell, digit)] = !solvedNearby;
            }
        }

        if (links.bivalueStrong || links.cellWeak) {
            for (uint32_t cell = 0; cell < kCells; ++cell) {
                vector<uint32_t> digits;
                for (uint32_t digit = 1; digit <= 9; ++digit) {
                    if (present[nodeId(cell, digit)]) digits.push_back(digit);
                }
                for (uint32_t a : digits) {
                    for (uint32_t b : digits) {
                        if (a == b) continue;
                        const uint32_t na = nodeId(cell, a);
                        const uint32_t nb = nodeId(cell, b);
                        if (links.bivalueStrong && digits.size() == 2) {
                            addStrong(na, nb);
                        } else if (links.cellWeak) {
                            addWeak(na, nb);
                        }
                    }
                }
            }
        }

        if (links.conjugateStrong || links.unitWeak) {
            for (uint32_t unit = 0; unit < 27; ++unit) {
                const auto cells = unitCells(unit);
                for (uint32_t digit = 1; digit <= 9; ++digit) {
                    vector<uint32_t> places;
                    for (uint32_t cell : cells) {
                        if (present[nodeId(cell, digit)])
                            places.push_back(cell);
                    }
                    for (uint32_t a : places) {
                        for (uint32_t b : places) {
                            if (a == b) continue;
                            const uint32_t na = nodeId(a, digit);
                            const uint32_t nb = nodeId(b, digit);
                            if (links.conjugateStrong && places.size() == 2) {
                                addStrong(na, nb);
                            } else if (links.unitWeak) {
                                addWeak(na, nb);
                            }
                        }
                    }
                }
            }
        }
    }

    // Everything that follows from assuming `start` is false (startTrue ==
    // false) or true (startTrue == true), following alternating links.
    void closure(uint32_t start, bool startTrue, vector<bool> &isTrue,
                 vector<bool> &isFalse) const {
        isTrue.assign(kNodes, false);
        isFalse.assign(kNodes, false);
        std::deque<std::pair<uint32_t, bool>> queue;
        (startTrue ? isTrue : isFalse)[start] = true;
        queue.push_back({start, startTrue});
        while (!queue.empty()) {
            const auto [node, truth] = queue.front();
            queue.pop_front();
            if (truth) {
                for (uint32_t m : weak[node]) {
                    if (isFalse[m]) continue;
                    isFalse[m] = true;
                    queue.push_back({m, false});
                }
            } else {
                for (uint32_t m : strong[node]) {
                    if (isTrue[m]) continue;
                    isTrue[m] = true;
                    queue.push_back({m, true});
                }
            }
        }
    }

    vector<bool> present;
    vector<vector<uint32_t>> strong;
    vector<vector<uint32_t>> weak;

   private:
    void addStrong(uint32_t a, uint32_t b) {
        strong[a].push_back(b);
        addWeak(a, b);
    }
    void addWeak(uint32_t a, uint32_t b) {
        for (uint32_t existing : weak[a]) {
            if (existing == b) return;
        }
        weak[a].push_back(b);
    }

    // units 0-8 rows, 9-17 columns, 18-26 ninths
    static vector<uint32_t> unitCells(uint32_t unit) {
        vector<uint32_t> cells;
        for (uint32_t i = 0; i < 9; ++i) {
            if (unit < 9) {
                cells.push_back(cellIndex(i, unit));
            } else if (unit < 18) {
                cells.push_back(cellIndex(unit - 9, i));
            } else {
                const uint32_t n = unit - 18;
                cells.push_back(
                    cellIndex((n % 3) * 3 + i % 3, (n / 3) * 3 + i / 3));
            }
        }
        return cells;
    }

    Board *mBoard;
};

bool removeCandidate(Board *board, uint32_t cell, uint32_t digit) {
    return board->at(cellX(cell), cellY(cell))->Remove(digit);
}

// Runs one pass: finds the first start node that yields eliminations, applies
// all eliminations proven by chains from that node and returns true. Returns
// false when no chain proves anything on the current board.
bool runChainPass(Board *board, const ChainLinks &links) {
    const ChainGraph graph(board, links);
    vector<bool> isTrue;
    vector<bool> isFalse;

    for (uint32_t start = 0; start < kNodes; ++start) {
        if (!graph.present[start]) continue;
        const uint32_t c0 = nodeCell(start);
        const uint32_t a = nodeDigit(start);
        bool changed = false;

        // "start is true => ... => start is false": start cannot be true.
        graph.closure(start, true, isTrue, isFalse);
        if (isFalse[start]) {
            changed |= removeCandidate(board, c0, a);
            if (changed) return true;
        }

        // "start is false => ... => end is true" proves "start or end".
        graph.closure(start, false, isTrue, isFalse);
        if (isTrue[start]) {
            // start is true whatever we assume: place it.
            changed |= board->at(cellX(c0), cellY(c0))
                           ->RemoveAllBut(vector<uint32_t>{a});
            if (changed) return true;
        }

        for (uint32_t end = 0; end < kNodes; ++end) {
            if (!isTrue[end] || end == start) continue;
            const uint32_t ck = nodeCell(end);
            const uint32_t b = nodeDigit(end);

            if (a == b) {
                // type 1: the digit is in c0 or in ck, so nowhere that sees
                // both.
                for (uint32_t cell = 0; cell < kCells; ++cell) {
                    if (cell == c0 || cell == ck) continue;
                    if (!graph.present[nodeId(cell, a)]) continue;
                    if (!seesEachOther(cell, c0) || !seesEachOther(cell, ck))
                        continue;
                    changed |= removeCandidate(board, cell, a);
                }
            } else if (c0 == ck) {
                // the cell is a or b, nothing else.
                changed |= board->at(cellX(c0), cellY(c0))
                               ->RemoveAllBut(vector<uint32_t>{a, b});
            } else if (seesEachOther(c0, ck)) {
                // type 2: c0 is a or ck is b. Either way c0 is not b and ck
                // is not a.
                if (graph.present[nodeId(c0, b)])
                    changed |= removeCandidate(board, c0, b);
                if (graph.present[nodeId(ck, a)])
                    changed |= removeCandidate(board, ck, a);
            }
        }
        if (changed) return true;
    }
    return false;
}

bool runChains(Board *board, const ChainLinks &links) {
    bool modified = false;
    while (runChainPass(board, links)) modified = true;
    return modified;
}

}  // namespace

bool XChainStrategy::run(Board *board) {
    return runChains(board, {false, true, false, true});
}

bool XYChainStrategy::run(Board *board) {
    return runChains(board, {true, false, false, true});
}

bool AlternatingInferenceChainStrategy::run(Board *board) {
    return runChains(board, {true, true, true, true});
}
