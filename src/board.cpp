#include "board.hpp"
const uint8_t Board::NUM_ROWS = 6;
const uint8_t Board::NUM_COLUMNS = 7;

uint64_t Board::encode() const {
  uint64_t bitboard = 0;
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    auto rowOpt = getLowestEmptyRow_(column);
    uint8_t row = rowOpt.has_value() ? rowOpt.value() + 1 : 0;
    bitboard |= (uint64_t)1 << (row * 7 + column); // Add flag above top of column
    for (; row < NUM_ROWS; ++row) {
      if (board_[row][column] == Color::BLACK) {
        bitboard |= (uint64_t)1 << ((row+1) * 7 + column); // Below the flag 0 represents Red (O) and 1 Black (X)
      }
    }
  }
  return bitboard;
}
void Board::decode(uint64_t bitboard) {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    bool flag = false; // bits below the flag represent pieces
    for (uint8_t row = 0; row < NUM_ROWS+1; ++row) {
      if(flag){
        if(bitboard & ((uint64_t)1 << (row * 7 + column))){
          board_[row-1][column] = Color::BLACK;
        } else {
          board_[row-1][column] = Color::RED;
        }
      } else if (bitboard & ((uint64_t)1 << (row * 7 + column))) {
        flag = true;
      }
    }
  }
}
std::optional<uint8_t> Board::getLowestEmptyRow_(uint8_t column) const {
  for (int row = NUM_ROWS - 1; row >= 0; --row){ // int because it needs to be briefly negative to break loop
    if (board_[row][column] == Color::EMPTY) {
      return row;
    }
  }
  return std::nullopt;
}
std::optional<uint8_t> Board::getHighestOccupiedRow_(uint8_t column) const {
  for (uint8_t row = 0; row < NUM_ROWS; ++row){
    if (board_[row][column] != Color::EMPTY) {
      return row;
    }
  }
  return std::nullopt;
}
bool Board::makeMove(const uint8_t column) {
  if(column >= NUM_COLUMNS) return false;

  if (auto lowestEmptyRow = getLowestEmptyRow_(column); lowestEmptyRow.has_value()) {
    board_[lowestEmptyRow.value()][column] = currentPlayer();
    movesMade_++;
    lastColumn_ = column;
    return true;
  }
  return false;
}
bool Board::undoMove(const uint8_t column) {
  if (auto highestOccupiedRow = getHighestOccupiedRow_(column); highestOccupiedRow.has_value()) {
    board_[highestOccupiedRow.value()][column] = Color::EMPTY;
    movesMade_--;
    return true;
  }
  return false;
}
void Board::print() const {
  print(std::cout);
}
void Board::print(std::ostream& out) const { 
  for (auto row : board_) {
    out << "|";
    for (auto space : row) {
      if (space == Color::EMPTY) {
        out << "-";
      } else if (space == Color::BLACK) {
        out << "X";
      } else if (space == Color::RED) {
        out << "O";
      }
      out << "|";
    }
    out << "\n";
  }

  out << "|";
  for (size_t i = 0; i < NUM_COLUMNS; i++) { // TODO: understand why had to leave size_t..
    out << i << "|";
  }
  out << '\n';
}
Color Board::currentPlayer() const {
  if(movesMade_ % 2 == 0){
    return Color::BLACK;
  } else {
    return Color::RED;
  }
}
Color Board::currentEnemy() const {
  if(movesMade_ % 2 == 0){
    return Color::RED;
  } else {
    return Color::BLACK;
  }
}
std::vector<uint8_t> Board::getMoves() const {
  std::vector<uint8_t> availableMoves;
  for (uint8_t col = 0; col < NUM_COLUMNS; ++col) {
    if (getLowestEmptyRow_(col).has_value()) {
      availableMoves.push_back(col);
    }
  }
  return availableMoves;
}
bool Board::checkWin() const {
  if(!lastColumn_.has_value()) return false;
  auto lastRowOpt = getHighestOccupiedRow_(lastColumn_.value());
  if (!lastRowOpt.has_value()) return false;

  auto lastRow = lastRowOpt.value();
  auto lastColumn = lastColumn_.value();

  int maxSquareSize = std::min(NUM_ROWS, NUM_COLUMNS);
  Color lastPiece = currentEnemy();

  // go down the column
  for (int i = 1; i < maxSquareSize; i++){
    uint8_t newRow = lastRow + i;

    // check if found a matching piece i below the given piece
    if (newRow < NUM_ROWS && board_[newRow][lastColumn] == lastPiece){
      uint8_t leftColumn = lastColumn - i;
      uint8_t rightColumn = lastColumn + i;

      // check left square
      if (leftColumn >= 0 && board_[lastRow][leftColumn] == lastPiece && board_[newRow][leftColumn] == lastPiece){
        return true;
      }

      // check right square
      if (rightColumn < NUM_COLUMNS && board_[lastRow][rightColumn] == lastPiece && board_[newRow][rightColumn] == lastPiece){
        return true;
      }
    }
  }

  return false;
}
bool Board::checkDraw() const {
  for (uint8_t i = 0; i < NUM_COLUMNS; i++){
    if (board_[0][i] == Color::EMPTY){
      return false;
    }
  }
  return true;
}
bool Board::gameOver() const {
return (checkWin() || checkDraw());
}