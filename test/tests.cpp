#include <gtest/gtest.h>

#include <fstream>

#include "board.hpp"
#include "cell.hpp"
#include "ninth.hpp"
#include "strategy.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

#if 1
#define CAP
#endif

class SudokuTest : public testing::Test {
   public:
    SudokuTest() {}

    std::string expectedOutput;

   protected:
    void SetUp() override {
#ifdef CAP
        testing::internal::CaptureStdout();
#endif
    }
    void TearDown() override {
#ifdef CAP
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_EQ(expectedOutput, output);
#endif
    }
};

std::string get_file_contents(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        return (std::string((std::istreambuf_iterator<char>(in)),
                            std::istreambuf_iterator<char>()));
    }
    throw(errno);
}

TEST_F(SudokuTest, CreateBoard) { Board board; }

TEST_F(SudokuTest, Coord) {
    Coord a(7, 1);
    Coord b(7, 1);
    ASSERT_EQ((a != b), false);
    ASSERT_EQ((a == b), true);

    Coord c(6, 1);
    Coord d(7, 2);
    ASSERT_EQ((a != c), true);
    ASSERT_EQ((a == c), false);
    ASSERT_EQ((a != d), true);
    ASSERT_EQ((a == d), false);
}

TEST_F(SudokuTest, Test001) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.001.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.001.output";

    const char *filename = infile.c_str();
    Board board(filename);

    board.print();
    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test002) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.002.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.002.output";

    const char *filename = infile.c_str();
    Board board(filename);

    ASSERT_EQ(board.at(7, 1)->count(), 1);

    DuplicatesStrategy s;
    s.run(&board);
    board.print();

    const uint32_t y = 1;
    for (uint32_t x = 0; x < 9; ++x) {
        if (x <= 2) {
            ASSERT_EQ(board.at(x, y)->count(), 7);
        } else if ((x == 3) || (x == 6)) {
            ASSERT_EQ(board.at(x, y)->count(), 7);
        } else if (x == 8) {
            ASSERT_EQ(board.at(x, y)->count(), 8);
        } else if (x != 7) {
            ASSERT_EQ(board.at(x, y)->count(), 8);
        } else {
            ASSERT_EQ(board.at(x, y)->count(), 1);
        }
    }

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test003) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.003.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.003.output";

    const char *filename = infile.c_str();
    Board board(filename);

    ASSERT_EQ(board.at(7, 1)->count(), 1);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test004) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.004.input";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);

    for (uint32_t x = 0; x < 3; ++x) {
        ASSERT_EQ(board.at(x, 4)->hasValue(1), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(2), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(3), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(4), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(5), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(6), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(9), true);
    }
}

TEST_F(SudokuTest, Test004_2) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.004.input";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);

    for (uint32_t x = 0; x < 3; ++x) {
        ASSERT_EQ(board.at(x, 4)->hasValue(1), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(2), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(3), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(4), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(5), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(6), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(9), true);
    }
}

TEST_F(SudokuTest, Test004_3) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.004.input";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);
    OnlyOnALineStrategy s3;
    s3.run(&board);

    for (uint32_t x = 0; x < 3; ++x) {
        ASSERT_EQ(board.at(x, 4)->hasValue(1), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(2), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(3), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(4), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(5), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(6), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(9), true);
    }

    for (uint32_t x = 6; x < 9; ++x) {
        ASSERT_EQ(board.at(x, 4)->hasValue(7), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(8), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(9), false);

        ASSERT_EQ(board.at(x, 3)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 3)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 3)->hasValue(9), true);
        ASSERT_EQ(board.at(x, 5)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 5)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 5)->hasValue(9), true);
    }
}

TEST_F(SudokuTest, Test005) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.005.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.005.output";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);
    OnlyOnALineStrategy s3;
    s3.run(&board);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test006) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.006.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.006.output";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);
    OnlyOnALineStrategy s3;
    s3.run(&board);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test007) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.007.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.007.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
    } while (run && limit > 0);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test008) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.008.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.008.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
    } while (run && limit > 0);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test009) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.009.input";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);

    // board.print();

    for (uint32_t x = 0; x < 3; ++x) {
        ASSERT_EQ(board.at(x, 4)->hasValue(1), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(2), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(3), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(4), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(5), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(6), false);
        ASSERT_EQ(board.at(x, 4)->hasValue(7), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(8), true);
        ASSERT_EQ(board.at(x, 4)->hasValue(9), true);
    }
}

TEST_F(SudokuTest, Test010) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.010.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.010.output";

    const char *filename = infile.c_str();
    Board board(filename);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test011) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.011.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.011.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
    } while (run && limit > 0);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test012) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.012.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.012.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
    } while (run && limit > 0);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test013) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.013.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.013.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
    } while (run && limit > 0);

    board.print(true);
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test013_2) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.013.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.013.2.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test013_3) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.013.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.013.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
        BoxLinesStrategy s5;
        run |= s5.run(&board);
    } while (run && limit > 0);

    board.print(true);
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test013_4) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.013.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.013_4.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;  // <- breaks here with s7
        run |= s3.run(&board);
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
        BoxLinesStrategy s5;
        run |= s5.run(&board);
        BoxLinesTwoValuesStrategy s6;
        run |= s6.run(&board);
        DoubleNinthStrategy s7;
        // run |= s7.run(&board);
    } while (run && limit > 0);

    board.print(true);
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test014) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.014.1.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.014.1.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test014_2) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.014.2.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.014.2.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test014_3) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.014.1.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.014.3.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;

        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test015) {
    string infile =
        string(ROOT_DIR) + "/test/ref/testcase.015_al_esgargot.input";
    string outfile =
        string(ROOT_DIR) + "/test/ref/testcase.015_al_esgargot.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run |= s.run(&board);
        OnlyOneStrategy s2;
        run |= s2.run(&board);
        OnlyOnALineStrategy s3;
        run |= s3.run(&board);
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test016) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.016.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.016.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test017) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.017.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.017.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test018) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.018.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.018.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test019) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.019.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.019.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test019_2) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.018.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.018.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test020_box_strategy) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.020.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.020.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        BoxLinesStrategy s5;
        run |= s5.run(&board);

    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test020_2_box_strategy) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.020.2.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.020.2.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        BoxLinesStrategy s5;
        run |= s5.run(&board);

    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test021_box_values_strategy) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.020.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.020.output";

    const char *filename = infile.c_str();
    Board board(filename);

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        // BoxLinesStrategy s5;
        // run |= s5.run(&board);
        BoxLinesTwoValuesStrategy s6;
        run |= s6.run(&board);

    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

TEST_F(SudokuTest, Test022) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.022.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.022.output";

    const char *filename = infile.c_str();
    Board board(filename);

    for (uint32_t i = 1; i <= 2; ++i) {
        board.at(8, 6)->Remove(i);
        for (uint32_t x = 6; x < 9; ++x) {
            for (uint32_t y = 7; y < 9; ++y) {
                board.at(x, y)->Remove(i);
            }
        }
    }

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DoubleNinthStrategy s;
        run |= s.run(&board);
    } while (run && limit > 0);

    board.print(true);
    printf("\n");
    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}
// Run every strategy in the usual order until nothing changes and check after
// each strategy that no candidate belonging to the known unique solution has
// been removed. Stops at the first violation so the report names the culprit.
static void runAllStrategiesVerified(Board &board, const char *solution) {
    auto verify = [&](const char *strategy) {
        for (uint32_t y = 0; y < 9; ++y) {
            for (uint32_t x = 0; x < 9; ++x) {
                const uint32_t v = solution[y * 9 + x] - '0';
                ASSERT_TRUE(board.at(x, y)->hasValue(v))
                    << strategy << " removed correct value " << v
                    << " from cell x=" << x << " y=" << y;
            }
        }
    };

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        verify("DuplicatesStrategy");
        if (testing::Test::HasFatalFailure()) return;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        verify("OnlyOneStrategy");
        if (testing::Test::HasFatalFailure()) return;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        verify("OnlyOnALineStrategy");
        if (testing::Test::HasFatalFailure()) return;
        DoubleLinesStrategy s4;
        run = s4.run(&board) || run;
        verify("DoubleLinesStrategy");
        if (testing::Test::HasFatalFailure()) return;
        BoxLinesStrategy s5;
        run = s5.run(&board) || run;
        verify("BoxLinesStrategy");
        if (testing::Test::HasFatalFailure()) return;
        BoxLinesTwoValuesStrategy s6;
        run = s6.run(&board) || run;
        verify("BoxLinesTwoValuesStrategy");
        if (testing::Test::HasFatalFailure()) return;
        DoubleNinthStrategy s7;
        run = s7.run(&board) || run;
        verify("DoubleNinthStrategy");
        if (testing::Test::HasFatalFailure()) return;
        // chains are expensive: only when the cheaper strategies are stuck
        if (run) continue;
        XChainStrategy s8;
        run = s8.run(&board) || run;
        verify("XChainStrategy");
        if (testing::Test::HasFatalFailure()) return;
        XYChainStrategy s9;
        run = s9.run(&board) || run;
        verify("XYChainStrategy");
        if (testing::Test::HasFatalFailure()) return;
        AlternatingInferenceChainStrategy s10;
        run = s10.run(&board) || run;
        verify("AlternatingInferenceChainStrategy");
        if (testing::Test::HasFatalFailure()) return;
    } while (run && limit > 0);
}

static uint32_t solvedCells(Board &board) {
    uint32_t solved = 0;
    for (uint32_t y = 0; y < 9; ++y) {
        for (uint32_t x = 0; x < 9; ++x) {
            if (board.at(x, y)->count() == 1) ++solved;
        }
    }
    return solved;
}

// Run every strategy on the hard sudoku 013 and verify that no strategy
// removes a value belonging to the known unique solution.
TEST_F(SudokuTest, Test013_5_all_strategies_correct) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.013.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.013_5.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "984351627257469813613287594378692451429815736"
        "561734982196573248832946175745128369";

    runAllStrategiesVerified(board, solution);
    EXPECT_EQ(81u, solvedCells(board));

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// The puzzles below are well known "very hard" sudokus that need techniques
// (chains, forcing nets or backtracking) the strategies do not implement.
// They are here to guarantee two things: the strategies stay sound on them
// (never remove a solution value) and the solver stops instead of looping.
// The golden outputs record how far the strategies get; they are expected
// to change when a new strategy makes more progress.

// AI Escargot (Arto Inkala, 2006)
TEST_F(SudokuTest, Test023_ai_escargot_sound_but_unsolved) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.023.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.023.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "162857493534129678789643521475312986913586742"
        "628794135356478219241935867897261354";

    runAllStrategiesVerified(board, solution);
    EXPECT_LT(solvedCells(board), 81u);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// "World's hardest sudoku" (Arto Inkala, 2012)
TEST_F(SudokuTest, Test024_inkala_2012_sound_but_unsolved) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.024.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.024.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "812753649943682175675491283154237896369845721"
        "287169534521974368438526917796318452";

    runAllStrategiesVerified(board, solution);
    EXPECT_LT(solvedCells(board), 81u);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// Golden Nugget
TEST_F(SudokuTest, Test025_golden_nugget_sound_but_unsolved) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.025.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.025.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "751846239892371465643259871238197546974562318"
        "165438927319684752527913684486725193";

    runAllStrategiesVerified(board, solution);
    EXPECT_LT(solvedCells(board), 81u);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// Platinum Blonde
TEST_F(SudokuTest, Test026_platinum_blonde_sound_but_unsolved) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.026.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.026.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "839465712146782953752391486391824675564173829"
        "287659341628537194913248567475916238";

    runAllStrategiesVerified(board, solution);
    EXPECT_LT(solvedCells(board), 81u);

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// X-Chain alone must reproduce the X-Wing eliminations of testcase 020 that
// BoxLinesStrategy finds in Test020_box_strategy.
TEST_F(SudokuTest, Test027_x_chain) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.020.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.027.output";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    XChainStrategy chain;
    EXPECT_TRUE(chain.run(&board));
    BoxLinesStrategy xwing;
    EXPECT_FALSE(xwing.run(&board)) << "X-Chain left an X-Wing behind";

    board.print(true);

    expectedOutput = get_file_contents(outfile.c_str());
}

// testcase 011 stalls with the basic strategies (Test011); an XY-Chain finishes
// it.
TEST_F(SudokuTest, Test028_xy_chain) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.011.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.028.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "531672984649831257827549613374928561185763429"
        "962415378753284196218396745496157832";

    bool run;
    int limit = 100;
    do {
        run = false;
        limit--;
        DuplicatesStrategy s;
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        if (run) continue;
        XYChainStrategy chain;
        run = chain.run(&board) || run;
    } while (run && limit > 0);

    for (uint32_t y = 0; y < 9; ++y) {
        for (uint32_t x = 0; x < 9; ++x) {
            const uint32_t v = solution[y * 9 + x] - '0';
            EXPECT_TRUE(board.at(x, y)->hasValue(v))
                << "wrong value at x=" << x << " y=" << y;
        }
    }
    EXPECT_EQ(81u, solvedCells(board));

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}

// testcase 029 needs an alternating inference chain that mixes bivalue-cell
// and conjugate-pair links: neither X-Chain nor XY-Chain alone solves it.
TEST_F(SudokuTest, Test029_alternating_inference_chain) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.029.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.029.output";

    const char *filename = infile.c_str();
    Board board(filename);

    const char *solution =
        "162754398439628571587139462651347829748592136"
        "293861754876215943314976285925483617";

    runAllStrategiesVerified(board, solution);
    EXPECT_EQ(81u, solvedCells(board));

    board.print();

    expectedOutput = get_file_contents(outfile.c_str());
}
