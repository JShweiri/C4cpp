#include "board.hpp"
#include "color.hpp"
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <functional>
using namespace std;

struct Data {
  double visits;
  // Black minus Red wins after visiting this state
  double bmrw;
};

std::unordered_map<uint64_t, Data> myMap;

int randomPolicy(const Board& board){
  auto v = board.getMoves();
  return v[rand() % v.size()];
}

// Broken. TODO: fix.
int UCBPolicy(Board& board) {
  auto c = sqrt(2.5); // Ensure `c` is of type double
  auto v = board.getMoves();
  auto parentEncoding = board.encode();

  optional<double> maxScore = nullopt; // Use double for maxScore
  int bestMove = 0;

  for (auto move : v) {
    board.makeMove(move);
    auto childEncoding = board.encode();
    
    // Check for visits to avoid division by zero and log of zero
    double childVisits = myMap[childEncoding].visits + 1;
    double parentVisits = myMap[parentEncoding].visits + 1; // Avoid log(0)

    double score = myMap[childEncoding].bmrw / childVisits
                  + c * sqrt(log(parentVisits) / childVisits);

    if (!maxScore.has_value() || score > *maxScore) {
      maxScore = score;
      bestMove = move;
    }

    board.undoMove(move);
  }
  
  return bestMove;
}

Color playout(Board& board, std::function<int(Board&)> policyMove){
  if (board.checkWin()){
    return board.currentEnemy();
  }
  if (board.checkDraw()){
    return Color::EMPTY;
  }

  int newMove = policyMove(board);
  board.makeMove(newMove);
  Color val = playout(board, policyMove);
  auto encoding = board.encode();
  myMap[encoding].visits++;
  if (val == Color::BLACK){
    myMap[encoding].bmrw += 1;
  } else if (val == Color::RED){
    myMap[encoding].bmrw -= 1;
  }
  board.undoMove(newMove);

  return val;
}

int treeSearch(Board &board, std::function<int(Board&)> policyMove) {
  auto moveList = board.getMoves();

  optional<int> maxVal = nullopt;
  int bestMove = 0;

  for (auto move : moveList) {
    board.makeMove(move);
    int total = 0;
    for (int i = 0; i < 50000; i++) {
      auto outcome = playout(board, policyMove);
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
  // cout << UCTPlayout(board);
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
      if(currentPlayer == Color::BLACK){
        move = treeSearch(board, UCBPolicy);
      } else {
        move = treeSearch(board, randomPolicy);
      }
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