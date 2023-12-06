#include "board.hpp"
#include "cell.hpp"
#include "ninth.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::unique_ptr;
using std::vector;

constexpr uint32_t SIZE_X = 3;
constexpr uint32_t SIZE_Y = 3;

constexpr uint32_t VALUE_0 = 48;

class Board::Private {
public:
  Private() {
    for (auto y = 0; y < 3; ++y) {
      for (auto x = 0; x < 3; ++x) {
        ninths.push_back(std::make_unique<Ninth>(Coord(x, y)));
      }
    }
  }

  Private(string buffer) {
    for (auto y = 0; y < 3; ++y) {
      for (auto x = 0; x < 3; ++x) {
        ninths.push_back(std::make_unique<Ninth>(Coord(x, y)));
      }
    }

    uint32_t index = 0;
    for (int i = 0; i < buffer.size() && (index < 81); ++i) {
      char c = buffer[i];
      if (c == '.') {
        index++;
        continue;
      } else if ((c >= '1') && (c <= '9')) {
        uint32_t x = index % (SIZE_X * 3);
        uint32_t y = index / (SIZE_X * 3);
        Cell *cell = at(x, y);
        cell->Set(static_cast<uint32_t>(c) - VALUE_0);
        index++;
      }
    }
  }

  Ninth *getNinth(uint32_t x, uint32_t y) {
    if ((x >= SIZE_X * 3) || (y >= SIZE_Y * 3))
      return nullptr;

    auto p = ninths[(x / 3) + (y / 3) * SIZE_X].get();
    return p;
  }

  Cell *at(uint32_t x, uint32_t y) {
    if ((x >= (SIZE_X * 3)) || ((y >= SIZE_Y * 3))) {
      printf("cell * at nullptr\n");
      throw std::exception();
    }

    auto ninth = getNinth(x, y);
    if (ninth) {
      auto cell = ninth->at(x, y);
      if (cell) {
        if ((cell->getCoord().x != x) || (cell->getCoord().y != y))
          throw std::exception();
        return cell;
      }
      throw std::exception();
    }

    printf("board::at() nullptr\n");
    throw std::exception();
  }

  std::vector<unique_ptr<Ninth>> ninths;
};

Board::Board() { mPriv = std::make_unique<Private>(); }

Board::~Board() {}

Board::Board(const char *filename) {
  mPriv = std::make_unique<Private>();
  auto fs = std::ifstream(filename, std::ios::in | std::ios::binary);
  if (!fs) {
    std::cout << "Error: file couldn't be opened\n";
    throw std::exception();
  }

  std::string buffer;
  std::string line;
  while (fs) {
    std::getline(fs, line);
    buffer += line;
  }
  mPriv = std::make_unique<Private>(buffer);
}

Ninth *Board::getNinth(Coord coord) { return getNinth(coord.x, coord.y); }

Ninth *Board::getNinth(uint32_t x, uint32_t y) { return mPriv->getNinth(x, y); }

Cell *Board::at(uint32_t x, uint32_t y) { return mPriv->at(x, y); }

char toChar(uint32_t value) {
  char c = value + VALUE_0;
  if (c == '0')
    c = '.';
  return c;
}

void Board::print(bool details) {
  if (!details) {
    for (int y = 0; y < 9; ++y) {
      if ((y > 0) && (y % 3 == 0))
        std::cout << "\n";
      for (int x = 0; x < 9; ++x) {
        if ((x > 0) && (x % 3 == 0))
          std::cout << " ";
        auto cell = at(x, y);
        if (cell) {
          std::cout << toChar(cell->Value());
        } else {
          throw std::exception();
        }
      }
      std::cout << "\n";
    }
  } else {
    for (int y = 0; y < 9; ++y) {
      if ((y > 0) && (y % 3 == 0))
        std::cout << "\n\n";
      for (uint32_t row = 0; row < 3; ++row) {
        for (int x = 0; x < 9; ++x) {
          at(x, y)->print(row);
          if ((x > 0) && ((x + 1) % 3 == 0) && (x < 8))
            std::cout << "      ";
        }
        std::cout << "\n";
      }
      if (y < 8)
        std::cout << "\n";
    }
  }
}

vector<Cell *> Board::getRow(const uint32_t y) {
  vector<Cell *> ret;
  for (uint32_t x = 0; x < 9; ++x) {
    ret.push_back(at(x, y));
  }
  return ret;
}

std::vector<Cell *> Board::getCol(const uint32_t x) {
  vector<Cell *> ret;
  for (uint32_t y = 0; y < 9; ++y) {
    ret.push_back(at(x, y));
  }
  return ret;
}
