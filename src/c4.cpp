#include "board.hpp"
#include "color.hpp"

#include <iostream>
using namespace std;

int randomMove(const Board& board){
  auto v = board.getMoves();
  return v[rand() % v.size()];
}

Color randomPlayout(Board& board){
  if (board.checkWin()){
    return board.currentEnemy();
  }
  if (board.checkDraw()){
    return Color::EMPTY;
  }

  int newMove = randomMove(board);
  board.makeMove(newMove);
  Color val = randomPlayout(board);
  board.undoMove(newMove);

  return val;
}

int monteCarloMove(Board &board) {
  auto moveList = board.getMoves();

  optional<int> maxVal = nullopt;
  int bestMove = 0;

  for (auto move : moveList) {
    board.makeMove(move);
    int total = 0;
    for (int i = 0; i < 100000; i++) {
      auto outcome = randomPlayout(board);
      if (outcome == board.currentEnemy()){
        total += 1;
      } else if (outcome == board.currentPlayer()){
        total -= 1;
      }
    }
    board.undoMove(move);
    cout << move << ": " << total << endl;

    if(!maxVal.has_value()){
      maxVal = total;
    }

    if (total > maxVal) {
      maxVal = total;
      bestMove = move;
    }
  }
  return bestMove;
}

int main() {
  srand(time(0));

  Board board;
  // cout << randomPlayout(board);
  bool humanX, humanO;

  cout << "Is X a human? (1 = yes, 0 = no): ";
  cin >> humanX;
  cout << "Is O a human? (1 = yes, 0 = no): ";
  cin >> humanO;

  while (!board.gameOver()){
    board.print();

    Color currentPlayer = board.currentPlayer();
    cout << "Player " << currentPlayer << "'s turn.\n";

    size_t move;  // TODO: understand why had to leave size_t..
    if ((currentPlayer == Color::BLACK && humanX) || (currentPlayer == Color::RED && humanO)) {
      cout << "Enter your move (0-" << Board::NUM_COLUMNS - 1 << "): ";
      cin >> move;
    } else {
      // AI makes a move
      move = monteCarloMove(board);
      cout << "AI chose move: " << move << endl;
    }

    if (board.makeMove(move)) {
      // Move successful
    } else {
      cout << "Invalid move. Try again.\n";
    }
  }

  board.print();

  if (board.checkWin()) {
    cout << "Player " << board.currentEnemy() << " wins!\n";
  } else {
    cout << "It's a draw!\n";
  }

  return 0;
}