
#include "strategy.hpp"
#include "board.hpp"
#include "cell.hpp"
#include "ninth.hpp"
#include <exception>
#include <set>
#include <vector>
#include <cstdio>

using std::vector;

bool clearRow(Board *board, Cell *cell)
{
    if (!board) throw std::exception();
    if (!cell)  throw std::exception();

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

bool clearCol(Board *board, Cell *cell)
{
    if (!board) throw std::exception();
    if (!cell)  throw std::exception();

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

bool clearNinth(Board *board, Cell *cell)
{
    if (!board) throw std::exception();
    if (!cell)  throw std::exception();

    bool ret = false;
    auto ninth = board->getNinth(cell->getCoord());
    auto c = ninth->getCoord();

    for (uint32_t y = c.y * 3; y < c.y * 3 + 3; ++y) {
        for (uint32_t x = c.x * 3; x < c.x * 3 + 3; ++x) {
            if (Coord(x, y) != cell->getCoord())
            {
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

bool DuplicatesStrategy::run(Board *board)
{
    bool modified = false;
    bool run;
    do {
        run = false;
        for (uint32_t x = 0; x < 9; ++x)
        {
            for (uint32_t y = 0; y < 9; ++y)
            {
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
    } while(run);
    return modified;
}

bool foundOnlyOnceNinth(Board *board, Ninth *ninth, uint32_t value) {
    int found = 0;
    for (uint32_t y = ninth->getCoord().y * 3; y < ninth->getCoord().y * 3 + 3; ++y) {
        for (uint32_t x = ninth->getCoord().x * 3; x < ninth->getCoord().x * 3 + 3; ++x) {

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
    for (uint32_t y = ninth->getCoord().y * 3; y < ninth->getCoord().y * 3 + 3; ++y) {
        for (uint32_t x = ninth->getCoord().x * 3; x < ninth->getCoord().x * 3 + 3; ++x) {
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
                if (i != value)
                    ret = cell->Remove(i) || ret;
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
                if (i != value)
                    ret = cell->Remove(i) || ret;
            }
        }
    }
    return ret;
}

bool OnlyOneStrategy::run(Board *board)
{
    bool modified = false;
    bool run;
    do {
        run = false;
        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check)
        {
            // check each ninth and check for only one value there
            for (uint32_t ny = 0; ny < 3; ++ny) {
                for (uint32_t nx = 0; nx < 3; ++nx) {
                    auto ninth = board->getNinth(nx * 3, ny * 3);
                    if (foundOnlyOnceNinth(board, ninth, check))
                    {
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
    } while(run);

    return modified;
}

bool foundOnlyOnRow(Board *board, Ninth *ninth, uint32_t y, uint32_t value)
{
    // verify that the value is found on the row
    bool valueFound = false;
    for (uint32_t x = 0; x < 9; ++x) {
        if (ninth != board->getNinth(x, y)) continue;
        auto cell = board->at(x, y);
        if (cell->hasValue(value))
            valueFound = true;
    }

    if (!valueFound) return false;

    // now let's go through other rows and see if it's found
    valueFound = false;
    for (uint32_t x = 0; x < 9; ++x) {
        for (uint32_t y2 = 0; y2 < 9; ++y2) {
            if (ninth != board->getNinth(x, y2)) continue;
            if (y2 == y) continue;

            auto cell = board->at(x, y2);
            if (cell->hasValue(value))
                return false;
        }
    }
    return true;
}

bool foundOnlyOnCol(Board *board, Ninth *ninth, uint32_t x, uint32_t value)
{
    // verify that the value is found on the row
    bool valueFound = false;
    for (uint32_t y = 0; y < 9; ++y) {
        if (ninth != board->getNinth(x, y)) continue;
        auto cell = board->at(x, y);
        if (cell->hasValue(value))
            valueFound = true;
    }

    if (!valueFound) return false;

    // now let's go through other rows and see if it's found
    valueFound = false;
    for (uint32_t y = 0; y < 9; ++y) {
        for (uint32_t x2 = 0; x2 < 9; ++x2) {
            if (ninth != board->getNinth(x2, y)) continue;
            if (x2 == x) continue;

            auto cell = board->at(x2, y);
            if (cell->hasValue(value))
                return false;
        }
    }
    return true;
}

bool OnlyOnALineStrategy::run(Board *board)
{
    bool modified = false;
    bool run;
    do {
        run = false;
        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check)
        {
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
                        if (foundOnlyOnCol(board, ninth, x, check))
                        {
                            for (uint32_t y = 0; y < 9; ++y) {
                                auto ninthToClear = board->getNinth(x, y);
                                if (ninthToClear->getCoord() == ninth->getCoord()) continue;

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
    } while(run);

    return modified;
}

bool DoubleLinesStrategy::run(Board *board)
{
    bool modified = false;
    bool run;
    do {
        run = false;

        vector<Ninth *> ninthLine;

        // check for value 'check' in each cell
        for (uint32_t check = 1; check <= 9; ++check)
        {
            for (uint32_t ny = 0; ny < 9; ny += 3) {
                ninthLine.clear();

                for (uint32_t nx = 0; nx < 9; nx += 3) {
                    ninthLine.push_back(board->getNinth(Coord(nx, ny)));
                }

                for (uint32_t it1 = 0; it1 < ninthLine.size() - 1; ++it1) {
                    for (uint32_t it2 = it1 + 1; it2 < ninthLine.size(); ++it2) {
                        for (uint32_t y = 0; y < 3; ++y) {
                            auto rows1 = ninthLine[it1]->hasValueOnRows(check);
                            auto rows2 = ninthLine[it2]->hasValueOnRows(check);

                            if ((rows1.size() == 2) && (rows1 == rows2)) {
                                // the third ninth cannot have this value on these two rows
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

                                // printf("removeValueFromRows: %u %u value %u \n", rows1[0], rows1[1], check);
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
                    for (uint32_t it2 = it1 + 1; it2 < ninthLine.size(); ++it2) {
                        for (uint32_t x = 0; x < 3; ++x) {
                            auto cols1 = ninthLine[it1]->hasValueOnCols(check);
                            auto cols2 = ninthLine[it2]->hasValueOnCols(check);

                            if ((cols1.size() == 2) && (cols1 == cols2)) {
                                // the third ninth cannot have this value on these two rows
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
    } while(run);

    return modified;
}

#if 0
bool LineNeedsANumberStrategy::run(Board *board)
{
    bool modified = false;
    bool run;
    do {
        run = false;



        modified |= true;
    } while(run);

    return modified;
}
#endif
