#include "board.hpp"
const uint8_t C4Game::NUM_ROWS = 6;
const uint8_t C4Game::NUM_COLUMNS = 7;
bool C4Game::makeMove(const uint8_t column) {
  if(column >= NUM_COLUMNS) return false;

  for (uint8_t row = 0; row < NUM_ROWS; ++row) {
    if (!(state_ & ((uint64_t)1 << (row * NUM_COLUMNS + column))) &&
        (state_ & ((uint64_t)1 << ((row+1) * NUM_COLUMNS + column)))) {
        // black
        state_ |= ((uint64_t)1 << (row * NUM_COLUMNS + column));
        // red
        state_ |= ((uint64_t)1 << (row * NUM_COLUMNS + column));
        state_ &= ~((uint64_t)1 << ((row+1) * NUM_COLUMNS + column));
        return true;
    }
  }
  return false;
}
bool C4Game::undoMove() {
  if(history_.empty()) return false;
  auto column = history_.top();

  for (uint8_t row = 0; row < NUM_ROWS; ++row) {
    if (state_ & ((uint64_t)1 << (row * NUM_COLUMNS + column))) {
      state_ &= ~((uint64_t)1 << (row * NUM_COLUMNS + column));
      state_ |= ((uint64_t)1 << ((row+1) * NUM_COLUMNS + column));
      history_.pop();
      return true;
    }
  }
  return false;
}
void C4Game::print() const {
  print(std::cout);
}

void C4Game::printRaw() const {
  std::cout << state_ << std::endl;
}

void C4Game::printBinary() const {
  for (uint8_t row = 0; row < NUM_ROWS+1; ++row) {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if(state_ & ((uint64_t)1 << (63-(row * NUM_COLUMNS + column)))){
      std::cout << 1;
    } else {
      std::cout << 0;
    }
  }
  }
  std::cout << std::endl;
}

// void C4Game::printBinaryFormatted() const {
//   std::cout << state_;
// }

void C4Game::print(std::ostream& out) const {
  bool flags[NUM_COLUMNS];
  for (uint8_t row = 0; row < NUM_ROWS; ++row) {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if(flags[column]){
      if(state_ & ((uint64_t)1 << (63-(row * NUM_COLUMNS + column)))){
        out << "X";
      } else {
        out << "O";
      }
    } else {
      if (state_ & ((uint64_t)1 << (63-(row * NUM_COLUMNS + column)))) {
      flags[column] = true;
      } else {
        out << "-";
      }
      }
  }
  out << "\n";
  }
}
Color C4Game::currentPlayer() const {
  if(history_.size() % 2 == 0){
    return 1;
  } else {
    return 0;
  }
}
Color C4Game::currentEnemy() const {
  if(history_.size() % 2 == 0){
    return 0;
  } else {
    return 1;
  }
}
std::vector<uint8_t> C4Game::getMoves() const {
  std::vector<uint8_t> availableMoves;
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if (!(state_ & ((uint64_t)1 << (0 * NUM_COLUMNS + column)))) {
      availableMoves.push_back(column);
    }
  }
  return availableMoves;
}
//... Continue changing to bitboard from here
bool C4Game::checkWin() const {
  if(history_.empty()) return false;
  auto column = history_.top();
  uint8_t row = 0;
  for (; row < NUM_ROWS; ++row) {
    if (state_ & ((uint64_t)1 << (row * NUM_COLUMNS + column))) {
      break;
    }
  }

  uint8_t maxSquareSize = std::min(NUM_ROWS, NUM_COLUMNS);
  Color lastPiece = currentEnemy();

  // go down the column
  for (uint8_t i = 1; i < maxSquareSize; i++){
    uint8_t bottomRow = row + i;

    // check if found a matching piece i below the given piece
    if (bottomRow < NUM_ROWS && (state_ >> (bottomRow * NUM_COLUMNS + column) & (uint64_t)1) == lastPiece){
      uint8_t leftColumn = column - i;
      uint8_t rightColumn = column + i;

      // check left square
      if (leftColumn >= 0 && (state_ >> (row * NUM_COLUMNS + leftColumn) & (uint64_t)1) == lastPiece && (state_ >> (bottomRow * NUM_COLUMNS + leftColumn) & (uint64_t)1) == lastPiece){
        return true;
      }

      // check right square
      if (rightColumn < NUM_COLUMNS && (state_ >> (row * NUM_COLUMNS + rightColumn) & (uint64_t)1) == lastPiece && (state_ >> (bottomRow * NUM_COLUMNS + rightColumn) & (uint64_t)1) == lastPiece){
        return true;
      }
    }
  }

  return false;
}
bool C4Game::checkDraw() const {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if (!(state_ & ((uint64_t)1 << (0 * NUM_COLUMNS + column)))) {
      return false;
    }
  }
  return true;
}
bool C4Game::gameOver() const {
return (checkWin() || checkDraw());
}