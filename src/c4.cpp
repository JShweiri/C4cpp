#include "board.hpp"
#include "color.hpp"

#include <iostream>
using namespace std;

int randomMove(const Board& board){
  auto v = board.getMoves();
  return v[rand() % v.size()];
}

int randomPlayout(Board& board){
  if (board.checkWin()){
    auto winningPiece = board.currentEnemy();
    return ((winningPiece == Color::BLACK) ? 1 : -1);
  }
  if (board.checkDraw()){
    return 0;
  }

  int newMove = randomMove(board);
  board.makeMove(newMove);
  int val = randomPlayout(board);
  board.undoMove(newMove);

  return val;
}

int main() {
  srand(time(0));

  Board board;
  bool humanX, humanO;

  cout << "Is X a human? (1 = yes, 0 = no): ";
  cin >> humanX;
  cout << "Is O a human? (1 = yes, 0 = no): ";
  cin >> humanO;

  while (!board.gameOver()){
    board.print();

    Color currentPlayer = board.currentPlayer();
    cout << "Player " << currentPlayer << "'s turn.\n";

    size_t move;
    if ((currentPlayer == Color::BLACK && humanX) || (currentPlayer == Color::RED && humanO)) {
      cout << "Enter your move (0-" << Board::NUM_COLUMNS - 1 << "): ";
      cin >> move;
    } else {
      // AI makes a move
      move = randomMove(board);
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