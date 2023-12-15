
#include "strategy.hpp"

#include "board.hpp"
#include "cell.hpp"
#include "ninth.hpp"
#include <assert.h>
#include <cstdio>
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

DoublesOnLine findDoubles(const vector<Cell *> &line, uint32_t value,
                          const uint32_t row_or_col) {
    DoublesOnLine ret;
    uint32_t found1;
    uint32_t found2;

    for (uint32_t index1 = 0; index1 < line.size() - 1; ++index1) {
        if (line[index1]->hasValue(value) && (line[index1]->count() > 1)) {
            found1 = index1;
            for (uint32_t index2 = index1 + 1; index2 < line.size(); ++index2) {
                if (line[index2]->hasValue(value) &&
                    (line[index2]->count() > 1)) {
                    found2 = index2;

                    for (uint32_t index3 = index2 + 1; index3 < line.size();
                         ++index3) {
                        if (line[index3]->hasValue(value)) {
                            return ret;
                        }
                    }
                    ret.found = true;
                    ret.index1 = found1;
                    ret.index2 = found2;
                    ret.value = value;
                    ret.row_or_col = row_or_col;
#if 0
          printf("findDoubles: found: true, index1: %u index2: %u value %u, "
                 "row_or_col %u\n",
                 found1, found2, value, row_or_col);
#endif
                    return ret;
                }
            }
        }
    }
    return ret;
}

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
    ValuesOnLine ret;
    uint32_t found1;
    uint32_t found2;

    for (uint32_t index1 = 0; index1 < line.size() - 1; ++index1) {
        if ((line[index1]->hasValue(value1) ||
             line[index1]->hasValue(value2)) &&
            (line[index1]->count() > 1)) {
            found1 = index1;
            const bool foundValue1 = line[index1]->hasValue(value1);
            const uint32_t check = (foundValue1) ? value2 : value1;
            ret.value1 = (foundValue1) ? value1 : value2;

            for (uint32_t index2 = index1 + 1; index2 < line.size(); ++index2) {
                if (line[index2]->hasValue(check) &&
                    (line[index2]->count() > 1)) {
                    found2 = index2;
                    ret.value2 = check;

                    for (uint32_t index3 = index2 + 1; index3 < line.size();
                         ++index3) {
                        if (line[index3]->hasValue(value1) ||
                            line[index3]->hasValue(value2)) {
                            return ret;
                        }
                    }
                    ret.found = true;
                    ret.index1 = found1;
                    ret.index2 = found2;
                    ret.row_or_col = row_or_col;
#if 0
                    printf(
                        "findValuesOnLine: found: true, index1: %u index2: %u "
                        "value1 %u, value2 %u "
                        "row_or_col %u\n",
                        found1, found2, value1, value2, row_or_col);
#endif
                    return ret;
                }
            }
        }
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
                            auto row2 = board->getCol(y2);
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
                                            run |= board->at(y3, found1.index1)
                                                       ->Remove(found1.value1);
                                            run |= board->at(y3, found1.index2)
                                                       ->Remove(found1.value1);
                                            run |= board->at(y3, found1.index1)
                                                       ->Remove(found1.value2);
                                            run |= board->at(y3, found1.index2)
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
                                }
                            }
                        }
                        // found suitable candidates
                        if (!fail && found2) {
                            n->at(coord1)->RemoveAllBut({check, check2});
                            n->at(coord2)->RemoveAllBut({check, check2});
                        }
                    }
                }
            }
        }

    } while (run);
}
