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
 * When a number is possible on one line in exactly two places
 * and the same number is possible in the exact same places on
 * another line, those 2 positions will hold the value on those
 * lines.
 */
class BoxLinesStrategy : public SudokuStrategy {
   public:
    BoxLinesStrategy(){};
    virtual ~BoxLinesStrategy(){};

    virtual bool run(Board *board);
};

/**
 * Same as BoxLinesStrategy, but with two different numbers.
 */
class BoxLinesTwoValuesStrategy : public SudokuStrategy {
   public:
    BoxLinesTwoValuesStrategy(){};
    virtual ~BoxLinesTwoValuesStrategy(){};

    virtual bool run(Board *board);
};

/**
 * When a two is found from only two cells in one ninth
 * those two cells cannot have any other values.
 */
class DoubleNinthStrategy : public SudokuStrategy {
public:
    DoubleNinthStrategy() { };
    virtual ~DoubleNinthStrategy() { };

    virtual bool run(Board *board);
};

#endif  // STRATEGY_HPP