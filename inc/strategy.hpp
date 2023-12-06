#ifndef STRATEGY_HPP
#define STRATEGY_HPP

class Board;

class SudokuStrategy {
public:
  SudokuStrategy(){};
  virtual ~SudokuStrategy(){};

  virtual bool run(Board *board) = 0;
};

/**
 * When a value is found, the same value cannot be in the same ninth, or the
 * same row/column
 */
class DuplicatesStrategy : public SudokuStrategy {
public:
  DuplicatesStrategy(){};
  virtual ~DuplicatesStrategy(){};

  virtual bool run(Board *board);
};

/**
 * When a value is only found from one cell in a ninth, no other value can be in
 * that cell.
 */
class OnlyOneStrategy : public SudokuStrategy {
public:
  OnlyOneStrategy(){};
  virtual ~OnlyOneStrategy(){};

  virtual bool run(Board *board);
};

/**
 * When a value is only found from one line in a ninth, the value cannot be on
 * that line on other ninths.
 */
class OnlyOnALineStrategy : public SudokuStrategy {
public:
  OnlyOnALineStrategy(){};
  virtual ~OnlyOnALineStrategy(){};

  virtual bool run(Board *board);
};

/**
 * When a value is found only from two lines in one ninth and
 * only from the same two lines on another ninth
 * the value cannot be on those lines on the third ninths.
 */
class DoubleLinesStrategy : public SudokuStrategy {
public:
  DoubleLinesStrategy(){};
  virtual ~DoubleLinesStrategy(){};

  virtual bool run(Board *board);
};

/**
 * When a full line needs a number, that number will be the only
 * possible value even if other values are otherwise possible.
 */
class BoxLinesStrategy : public SudokuStrategy {
public:
  BoxLinesStrategy(){};
  virtual ~BoxLinesStrategy(){};

  virtual bool run(Board *board);
};

#endif // STRATEGY_HPP