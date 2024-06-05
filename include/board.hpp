#ifndef BOARD_HPP
#define BOARD_HPP
#include <cstddef>
#include <iostream>
#include <optional>
#include <vector>
#include <algorithm>
#include "color.hpp"
class Board {
 public:
  bool makeMove(const size_t column);
  bool undoMove(const size_t column);
  void print() const;
  void print(std::ostream& out) const;
  Color currentPlayer() const;
  Color currentEnemy() const;
  std::vector<size_t> getMoves() const;
  bool checkWin() const;
  bool checkDraw() const;
  bool gameOver() const;

  static size_t NUM_ROWS;
  static size_t NUM_COLUMNS;
 private:
  std::optional<size_t> getLowestEmptyRow_(size_t column) const;
  std::optional<size_t> getHighestOccupiedRow_(size_t column) const;
  std::vector<std::vector<Color>> board_{NUM_ROWS, std::vector<Color>(NUM_COLUMNS, Color::EMPTY)};
  size_t movesMade_ = 0;
  std::optional<size_t> lastColumn_ = std::nullopt;
};
#endif