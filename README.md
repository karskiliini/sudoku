# Sudoku Solver
Sudoku solver based on running different strategies on a given sudoku.

# State of the project
Currently the solver is not able to solve the most difficult sudokus due to not having the required advanced strategies in place yet.

# Build Instructions
```
$ cmake -S . -B build
$ make -C build
```
# Run the app
```
$ ./build/solverApp test/ref/testcase.013.input
9.4 .5. .2.
..7 ... .1.
6.3 28. ...

... .9. ..1
... 8.. .36
5.. 7.. ..2

1.. ..3 .4.
... ..6 ..5
.4. ..8 .69
```

# Run tests
```
$ make -C build
$ make -C build test
```
or
```
$ cd build && make && ctest
```

# Example sudoku

## Initial state
```
... 26. 7.1
68. .7. .9.
19. ..4 5..

82. 1.. .4.
..4 6.2 9..
.5. ..3 .28

..9 3.. .74
.4. .5. .36
7.3 .18 ...
```

## Solved
```
435 269 781
682 571 493
197 834 562

826 195 347
374 682 915
951 743 628

519 326 874
248 957 136
763 418 259
```