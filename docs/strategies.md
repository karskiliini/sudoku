# Solving strategies

The solver works on a 9x9 board where every unsolved cell keeps a list of
candidate digits. A strategy looks at the candidates, removes the ones it can
prove impossible and returns `true` when it changed something. The driver runs
the strategies in order until a full pass changes nothing.

A strategy is **sound** when it never removes a digit that belongs to the
puzzle's solution. Every strategy below is tested for soundness, see
[Testing](#testing).

## Vocabulary

| Term | Meaning |
|---|---|
| unit | a row, a column or a 3x3 box (called a *ninth* in the code) |
| peers | the 20 cells that share a unit with a given cell |
| candidate | a digit still possible in an unsolved cell |
| bivalue cell | a cell with exactly two candidates |
| conjugate pair | a digit that is possible in exactly two cells of one unit |

## Basic strategies

These are the strategies in `src/strategy.cpp` that look at one or two units at
a time.

| Class | Rule |
|---|---|
| `DuplicatesStrategy` | A solved cell removes its digit from all peers. |
| `OnlyOneStrategy` | A digit possible in only one cell of a unit is placed there (hidden single). |
| `OnlyOnALineStrategy` | A digit confined to one row or column inside a box is removed from that line outside the box (pointing pair / box-line reduction). |
| `DoubleLinesStrategy` | A digit confined to the same two lines in two boxes of a band is removed from those lines in the third box. |
| `BoxLinesStrategy` | A digit possible in exactly two cells of one line and in the same two positions of another line is removed from those positions elsewhere (X-Wing). |
| `BoxLinesTwoValuesStrategy` | Same as above for two digits that together occupy exactly two cells of each line. |
| `DoubleNinthStrategy` | Two digits possible in only the same two cells of a box restrict those cells to the two digits (hidden pair). |

Two details matter for soundness. A digit that is already solved somewhere on
a line can never form a pair on that line, so `findValuesOnLine` and
`findDoubles` report *not found* when they meet a solved cell holding the
digit. Without that guard a solved cell plus stale candidates that
`DuplicatesStrategy` has not removed yet looks like a conjugate pair and
produces a false X-Wing.

## Chain strategies

Everything above is a local pattern. Chains reason about consequences instead.

### Implication graph

Every candidate `(cell, digit)` is a node. There are two kinds of link:

* **strong link**, "if A is false then B is true". Sources:
  * the two candidates of a bivalue cell
  * the two cells of a conjugate pair
* **weak link**, "if A is true then B is false". Sources:
  * two different candidates of one cell
  * the same digit in two cells of one unit

A strong link is always usable as a weak link.

A chain alternates strong and weak links: `A false ⇒ B true ⇒ C false ⇒ D true
⇒ …`. Because every step is a valid implication, any chain that starts with
"A false" and reaches "Z true" proves **A or Z**: at least one of the two ends
is true.

The engine (`ChainGraph` and `runChainPass` in `src/strategy.cpp`) builds the
graph once per pass and, for every start candidate, does a breadth-first walk
over states `(node, true/false)`. From a *false* state it follows strong links,
from a *true* state it follows weak links. The set of nodes reached in the
*true* state is exactly the set of chain ends, so no explicit path search is
needed and repeated nodes are harmless.

### Eliminations

For a start `A = (c0, a)` and every reached end `Z = (ck, b)`:

| Situation | Conclusion |
|---|---|
| same digit, `a == b` | `a` is in `c0` or in `ck`, so `a` is removed from every cell that sees both. |
| different digits, `c0` and `ck` see each other | `c0` is `a` or `ck` is `b`; either way `c0` is not `b` and `ck` is not `a`. |
| different digits, same cell | the cell is `a` or `b`; every other candidate is removed. |
| the walk from "A true" reaches "A false" | `A` contradicts itself and is removed. |
| the walk from "A false" reaches "A true" | `A` holds whatever we assume and is placed. |

After the first start candidate that yields eliminations the graph is rebuilt,
so every chain is always evaluated on the current board.

### Stale candidates

The graph treats a candidate as absent when a peer of its cell is already
solved with that digit. This is the same guard the basic strategies use, and it
means the chains are sound even when they run before `DuplicatesStrategy` has
cleaned up after a placement.

### The three strategies

They share the engine and differ only in the links they may use.

| Class | Strong links | Weak links | Covers |
|---|---|---|---|
| `XChainStrategy` | conjugate pairs | same digit in a unit | simple coloring, X-Wing, Skyscraper, Two-String Kite, X-Cycle |
| `XYChainStrategy` | bivalue cells | same digit in a unit | XY-Wing, XY-Chain |
| `AlternatingInferenceChainStrategy` | both | both | everything above plus mixed chains |

`AlternatingInferenceChainStrategy` subsumes the other two. The narrower
strategies exist because they are cheaper and because their eliminations are
easier for a person to follow.

Chains cost more than the basic strategies, so the recommended driver runs
them only when a full pass of the basic strategies changed nothing:

```cpp
do {
    run = false;
    // basic strategies ...
    if (run) continue;
    XChainStrategy s8;                    run = s8.run(&board) || run;
    XYChainStrategy s9;                   run = s9.run(&board) || run;
    AlternatingInferenceChainStrategy s10; run = s10.run(&board) || run;
} while (run && limit > 0);
```

## Testing

There are two kinds of test in `test/tests.cpp`.

**Golden output tests** run a fixed set of strategies on an input in `test/ref`
and compare the printed board with a `.output` file. They pin the exact
behaviour and have to be regenerated on purpose when a strategy gets stronger.

**Soundness tests** know the unique solution of the puzzle and check after
*every* strategy call that no solution digit has been removed. The helper
`runAllStrategiesVerified` does this for the full strategy chain and stops at
the first violation so the failure names the culprit. Solutions were obtained
with an independent brute-force solver and each puzzle was confirmed to have
exactly one solution.

| Test | Purpose |
|---|---|
| `Test013_5_all_strategies_correct` | hard puzzle 013 is solved by the full chain, soundly |
| `Test023` – `Test026` | AI Escargot, Inkala 2012, Golden Nugget, Platinum Blonde: the solver stays sound and stops instead of looping, but does not solve them |
| `Test027_x_chain` | X-Chain alone reproduces the X-Wing eliminations of testcase 020 |
| `Test028_xy_chain` | testcase 011 stalls with the basic strategies and is finished by XY-Chain |
| `Test029_alternating_inference_chain` | testcase 029 is solved by AIC but by neither X-Chain nor XY-Chain alone |

### Results on a 127 puzzle corpus

Measured with a harness that verifies soundness after every strategy call.

| Strategy set | Fully solved | Unsound removals |
|---|---|---|
| basic strategies only | 77 / 127 | 0 |
| basic + X-Chain | 81 / 127 | 0 |
| basic + XY-Chain | 94 / 127 | 0 |
| basic + AIC | 121 / 127 | 0 |

The remaining six puzzles include the four in `Test023` – `Test026`. They
need techniques the solver does not have.

## What the solver cannot do

* **Grouped and ALS links.** The chain engine only links single candidates.
  Grouped nodes (several cells of a box on one line) and almost-locked-set
  links would extend the reach of AIC without new theory.
* **Forcing nets.** A forcing net assumes a candidate, propagates through
  several cells at once and keeps the conclusions that hold under every
  assumption. The hardest published puzzles need nets several levels deep.
* **Backtracking.** There is no search. A backtracking fallback seeded from the
  reduced candidate grid is the pragmatic way to guarantee a solution on any
  input and would make the strategies above an optimisation rather than a
  requirement.

## Known issues

* `Cell::RemoveAllBut(uint32_t)` never removes digit 9 (its loop runs 0..8).
  It is unused; every caller uses the `std::vector` overload. Delete or fix it.
* `src/main.cpp` runs only the first four basic strategies, so the command line
  app is weaker than the test driver.
