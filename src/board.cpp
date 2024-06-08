#include "board.hpp"
const uint8_t C4Game::NUM_ROWS = 6;
const uint8_t C4Game::NUM_COLUMNS = 7;
using namespace std;

// This is the 49 size index. no pieces should be placed on the 0th row, just flags.
inline uint64_t C4Game::rowColToIndex(uint8_t row, uint8_t column, uint8_t numColumns) const {
    return 63 - (row * numColumns + column);
}

inline uint64_t C4Game::rowColToOHE(uint8_t row, uint8_t column, uint8_t numColumns) const {
    return (uint64_t)1 << rowColToIndex(row, column, numColumns);
}

// can I make this at compile time?
C4Game::C4Game() {
    state_ = 0;
    for (uint8_t i = 0; i < NUM_COLUMNS; ++i) {
        state_ |= rowColToOHE(NUM_ROWS+1, i, NUM_ROWS);
    }
}

bool C4Game::makeMove(const uint8_t column) {
  //Figure out why second part is necessary.. shouldnt this already be implied by the logic?
  if(column >= NUM_COLUMNS || (state_ & rowColToOHE(0,column))) return false;
  for (uint8_t row = 0; row < NUM_ROWS; ++row) {
    if (!(state_ & rowColToOHE(row,column)) &&
         (state_ & rowColToOHE(row+1,column))) {
        if(currentPlayer() == BLACK){
          state_ |= rowColToOHE(row,column);
        } else {
          state_ |= rowColToOHE(row,column);
          state_ &= ~rowColToOHE(row+1,column);
        }
        history_.push(column);
        return true;
    }
  }
  return false;
}
bool C4Game::undoMove() {
  if(history_.empty()) return false;
  auto column = history_.top();

  for (uint8_t row = 0; row < NUM_ROWS; ++row) {
    if (state_ & rowColToOHE(row,column)) {
      state_ &= ~rowColToOHE(row,column);
      state_ |= rowColToOHE(row+1,column);
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

void C4Game::printBinary(bool showIndexes, bool showExtra, char lineSep) const {
  for (uint8_t row = 0; row < NUM_ROWS+1; ++row) {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if(showIndexes) cout << "(" << rowColToIndex(row, column) << ")";
    if(state_ & rowColToOHE(row,column)){
      std::cout << 1;
    } else {
      std::cout << 0;
    }
  }
  std::cout << lineSep;
  }
  if(showExtra){
  std::cout << "|";
  for(int i = 0; i < 64-(NUM_ROWS+1)*NUM_COLUMNS; i++) std::cout << "0";
  }
  std::cout << std::endl;
}

// void C4Game::printBinaryFormatted() const {
//   std::cout << state_;
// }

void C4Game::print(std::ostream& out) const {
  bool flags[NUM_COLUMNS] = {};
  for (uint8_t row = 0; row < NUM_ROWS+1; ++row) {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if(flags[column]){
      if(state_ & rowColToOHE(row,column)){
        out << "X";
      } else {
        out << "O";
      }
    } else {
      if(row > 0){
        out << "-";
      }
      if(state_ & rowColToOHE(row,column)){
        flags[column] = true;
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
    if (!(state_ & rowColToOHE(0,column))) {
      availableMoves.push_back(column);
    }
  }
  return availableMoves;
}
bool C4Game::checkWin() const {
  if(history_.empty()) return false;
  auto column = history_.top();
  uint8_t row = 0;
  for (; row < NUM_ROWS; ++row) {
    if (state_ & rowColToOHE(row,column)) {
      break;
    }
  }
  row+=1;

  uint8_t maxSquareSize = std::min(NUM_ROWS, NUM_COLUMNS);
  Color lastPiece = currentEnemy();

  cout << "last piece" << +lastPiece << endl;

  // go down the column
  for (uint8_t i = 1; i < maxSquareSize; i++){
    uint8_t bottomRow = row + i;

  // need to check above the spot to see if any 1s to know if spot is a piece

  // is storing array of heights better computationally?
  // 3 bits * 7 cols. can use a 32 bit num.

  // use mod arith somehow?

    // check if found a matching piece i below the given piece
    if (bottomRow < NUM_ROWS+1 && (state_ & rowColToOHE(bottomRow,column)) == lastPiece){
        std::cout << "size found: " << +i;

      uint8_t leftColumn = column - i;
      uint8_t rightColumn = column + i;

        uint64_t lvert = 0;
  for (int i = 0; i < NUM_ROWS; ++i) {
    lvert |= rowColToOHE(i, leftColumn);
  }

    uint64_t rvert = 0;
  for (int i = 0; i < NUM_ROWS; ++i) {
    rvert |= rowColToOHE(i, rightColumn);
  }

      auto pal = (+(lvert & state_) > +rowColToOHE(row,leftColumn));
      auto par = (+(rvert & state_) > +rowColToOHE(row,rightColumn));
      std::cout << "piece above l: " << pal << std::endl;
      std::cout << "piece above r: " << par << std::endl;
      // check left square
      if (leftColumn >= 0 && pal && ((state_ & rowColToOHE(row,leftColumn)) == lastPiece) && ((state_ & rowColToOHE(bottomRow,leftColumn)) == lastPiece)){
        std::cout << "left";
        return true;
      }

      // check right square
      if (rightColumn < NUM_COLUMNS && par && ((state_ & rowColToOHE(row,rightColumn)) == lastPiece) && ((state_ & rowColToOHE(bottomRow,rightColumn)) == lastPiece)){
        std::cout << "right";
        return true;
      }
    }
  }

  return false;
}
bool C4Game::checkDraw() const {
  for (uint8_t column = 0; column < NUM_COLUMNS; ++column) {
    if (!(state_ & rowColToOHE(0,column))) {
      return false;
    }
  }
  return true;
}
bool C4Game::gameOver() const {
return (checkWin() || checkDraw());
}