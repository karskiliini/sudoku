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
   protected:
    void SetUp() override {}
    // void TearDown() override {}
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
    testing::internal::CaptureStdout();

    string infile = string(ROOT_DIR) + "/test/ref/testcase.001.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.001.output";

    const char *filename = infile.c_str();
    Board board(filename);

    board.print();

    std::string output = testing::internal::GetCapturedStdout();

    string expectedOutput = get_file_contents(outfile.c_str());

    ASSERT_EQ(expectedOutput, output);
}

TEST_F(SudokuTest, Test002) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif
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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test003) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif
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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test004) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.004.input";

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

TEST_F(SudokuTest, Test004_2) {
    string infile = string(ROOT_DIR) + "/test/ref/testcase.004.input";

    const char *filename = infile.c_str();
    Board board(filename);

    DuplicatesStrategy s;
    s.run(&board);
    OnlyOneStrategy s2;
    s2.run(&board);

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
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test006) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test007) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
    } while (run && limit > 0);

    board.print();
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test008) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
    } while (run && limit > 0);

    board.print();
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
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
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

    string infile = string(ROOT_DIR) + "/test/ref/testcase.010.input";
    string outfile = string(ROOT_DIR) + "/test/ref/testcase.010.output";

    const char *filename = infile.c_str();
    Board board(filename);

    board.print();
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test011) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
    } while (run && limit > 0);

    board.print();
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test012) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
    } while (run && limit > 0);

    board.print();
#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test013) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
    } while (run && limit > 0);

    board.print(true);
    board.print();

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test013_2) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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
        run = s.run(&board) || run;
        OnlyOneStrategy s2;
        run = s2.run(&board) || run;
        OnlyOnALineStrategy s3;
        run = s3.run(&board) || run;
        DoubleLinesStrategy s4;
        run |= s4.run(&board);
    } while (run && limit > 0);

    board.print(true);
    board.print();

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test014) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#if 1
    board.print(true);
#else
    board.print();
#endif

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test014_2) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#if 1
    board.print(true);
#else
    board.print();
#endif

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test014_3) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#if 1
    board.print(true);
    printf("\n");
    board.print();
#else
    board.print();
#endif

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test015) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test016) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test017) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test018) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test019) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test019_2) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test020_box_strategy) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}

TEST_F(SudokuTest, Test020_2_box_strategy) {
#ifdef CAP
    testing::internal::CaptureStdout();
#endif

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

#ifdef CAP
    std::string output = testing::internal::GetCapturedStdout();
#endif

    string expectedOutput = get_file_contents(outfile.c_str());
#ifdef CAP
    ASSERT_EQ(expectedOutput, output);
#endif
}
