#ifndef BOARD_HPP
#define BOARD_HPP
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>

#define BLACK 1
#define RED 0
typedef uint8_t Color; // typedefed in case we want to switch to a class. Also good for self explanation instead of just seeing uint8_ts everywhere. no context needed
typedef uint64_t Board;// ^
class C4Game {
 public:
  C4Game();
  inline bool rowColEquals(uint64_t state, uint8_t row, uint8_t column, uint8_t value, uint8_t numColumns = NUM_COLUMNS) const;
  inline uint64_t rowColToIndex(uint8_t row, uint8_t column, uint8_t numColumns = NUM_COLUMNS) const;
  inline uint64_t rowColToOHE(uint8_t row, uint8_t column, uint8_t numColumns = NUM_COLUMNS) const;
  bool makeMove(const uint8_t column);
  bool undoMove();
  void print() const;
  void print(std::ostream& out) const;
  void printRaw() const;
  void printBinary(bool showIndexes = true, bool showExtra = true, char lineSep = '\n') const;
  Color currentPlayer() const;
  Color currentEnemy() const;
  std::vector<uint8_t> getMoves() const;
  bool checkWin() const; // Checks if currentEnemy won.
  bool checkDraw() const;
  bool gameOver() const;

  const static uint8_t NUM_ROWS;
  const static uint8_t NUM_COLUMNS;
 private:
  Board state_;
  std::stack<uint8_t> history_;
};
#endif