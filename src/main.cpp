
#include "board.hpp"
#include "strategy.hpp"
#include <string>
#include <iostream>

using std::string;

int main(int argc, const char *argv[]) {

    if (argc < 2)
    {
        std::cout << "Error: input filename must be given.\n";
        return -1;
    }

    const char *filename = argv[1];

    try {
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

        board.print();
    }
    catch(...) {
        std::cout << "Exception caught, abort.\n";
    }
    return 0;
}