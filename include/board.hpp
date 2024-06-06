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
  bool makeMove(const uint8_t column);
  bool undoMove(const uint8_t column);
  void print() const;
  void print(std::ostream& out) const;
  Color currentPlayer() const;
  Color currentEnemy() const;
  std::vector<uint8_t> getMoves() const;
  bool checkWin() const;
  bool checkDraw() const;
  bool gameOver() const;
  uint64_t encode() const;
  void decode(uint64_t bitboard);

  const static uint8_t NUM_ROWS;
  const static uint8_t NUM_COLUMNS;
 private:
  std::optional<uint8_t> getLowestEmptyRow_(uint8_t column) const;
  std::optional<uint8_t> getHighestOccupiedRow_(uint8_t column) const;
  std::vector<std::vector<Color>> board_{NUM_ROWS, std::vector<Color>(NUM_COLUMNS, Color::EMPTY)};
  uint8_t movesMade_ = 0;
  std::optional<uint8_t> lastColumn_ = std::nullopt;
};
#endif