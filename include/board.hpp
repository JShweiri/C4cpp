#ifndef BOARD_HPP
#define BOARD_HPP
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>
  typedef uint8_t Color; // typedefed in case we want to switch to a class. Also good for self explanation instead of just seeing uint8_ts everywhere. no context needed
  typedef uint64_t Board;// ^
class C4Game {
 public:
  bool makeMove(const uint8_t column);
  bool undoMove();
  void print() const;
  void print(std::ostream& out) const;
  void printRaw() const;
  void printBinary() const ;
  Color currentPlayer() const;
  Color currentEnemy() const;
  std::vector<uint8_t> getMoves() const;
  bool checkWin() const; // Checks if currentEnemy won.
  bool checkDraw() const;
  bool gameOver() const;

  const static uint8_t NUM_ROWS;
  const static uint8_t NUM_COLUMNS;
 private:
  std::optional<uint8_t> getLowestEmptyRow_(uint8_t column) const;
  std::optional<uint8_t> getHighestOccupiedRow_(uint8_t column) const;
  Board state_ = ((int)pow(2, NUM_COLUMNS) - 1) << (64-49);
  std::stack<uint8_t> history_;
};
#endif