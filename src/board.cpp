#include "board.hpp"
size_t Board::NUM_ROWS = 6;
size_t Board::NUM_COLUMNS = 7;

std::optional<size_t> Board::getLowestEmptyRow_(size_t column) const {
  for (int row = NUM_ROWS - 1; row >= 0; --row){ // int because it needs to be briefly negative to break loop
    if (board_[row][column] == Color::EMPTY) {
      return row;
    }
  }
  return std::nullopt;
}
std::optional<size_t> Board::getHighestOccupiedRow_(size_t column) const {
  for (size_t row = 0; row < NUM_ROWS; ++row){
    if (board_[row][column] != Color::EMPTY) {
      return row;
    }
  }
  return std::nullopt;
}
bool Board::makeMove(const size_t column) {
  if(column >= NUM_COLUMNS) return false;

  if (auto lowestEmptyRow = getLowestEmptyRow_(column); lowestEmptyRow.has_value()) {
    board_[lowestEmptyRow.value()][column] = currentPlayer();
    movesMade_++;
    lastColumn_ = column;
    return true;
  }
  return false;
}
bool Board::undoMove(const size_t column) {
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
  for (size_t i = 0; i < NUM_COLUMNS; i++) {
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
std::vector<size_t> Board::getMoves() const {
  std::vector<size_t> availableMoves;
  for (size_t col = 0; col < NUM_COLUMNS; ++col) {
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
    size_t newRow = lastRow + i;

    // check if found a matching piece i below the given piece
    if (newRow < NUM_ROWS && board_[newRow][lastColumn] == lastPiece){
      size_t leftColumn = lastColumn - i;
      size_t rightColumn = lastColumn + i;

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
  for (size_t i = 0; i < NUM_COLUMNS; i++){
    if (board_[0][i] == Color::EMPTY){
      return false;
    }
  }
  return true;
}
bool Board::gameOver() const {
return (checkWin() || checkDraw());
}