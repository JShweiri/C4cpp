#include "board.hpp"
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <optional>
#include <cstdlib>
#include <ctime>

using namespace std;

// Avoid Recursion

// use bitboard everywhere

// fix MCTS

// Implement NN Heuristic :)

// struct Data {
//   double visits = 0;
//   double blackWins = 0;
//   double redWins = 0;
// };

// std::unordered_map<uint64_t, Data> myMap;
// std::mutex mapMutex;

// uint8_t randomPolicy(const C4Game& board) {
//   auto v = board.getMoves();
//   return v[rand() % v.size()];
// }



// Is this faster?
// Random number generator
// random_device rd;  
// mt19937 gen(rd()); 
// uniform_int_distribution<> distrib(0, numeric_limits<int>::max());

// // Function for random move selection
// uint8_t randomPolicy(const C4Game& board) {
//     auto moves = board.getMoves();
//     return moves[distrib(gen) % moves.size()];
// }


// uint8_t UCBPolicy(C4Game& board) {
//   const double c = 10000.0; // Ensure `c` is of type double
//   auto v = board.getMoves();
//   auto parentEncoding = board.encode();

//   optional<double> maxScore = nullopt; // Use double for maxScore
//   vector<int> bestMoves;

//   double parentVisits;
//   {
//     std::lock_guard<std::mutex> lock(mapMutex);
//     parentVisits = myMap[parentEncoding].visits + 2; // Avoid log(0)
//   }

//   for (auto move : v) {
//     board.makeMove(move);
//     auto childEncoding = board.encode();

//     double childVisits, wins;
//     {
//       std::lock_guard<std::mutex> lock(mapMutex);
//       childVisits = myMap[childEncoding].visits + 2; // Avoid division by zero
//       wins = (board.currentPlayer() == Color::BLACK ? myMap[childEncoding].blackWins : myMap[childEncoding].redWins);
//     }

//     double score = wins / childVisits + c * sqrt(log(parentVisits) / childVisits);

//     if (!maxScore.has_value() || score > *maxScore) {
//       maxScore = score;
//       bestMoves.clear();
//       bestMoves.push_back(move);
//     } else if (score == *maxScore) {
//       bestMoves.push_back(move);
//     }

//     board.undoMove();
//   }

//   if (!bestMoves.empty()) {
//     return bestMoves[rand() % bestMoves.size()];
//   }

//   return bestMoves[0]; // Fallback in case bestMoves is empty
// }

// Color playout(C4Game& board, std::function<int(C4Game&)> movePolicy) {
//   if (board.checkWin()) {
//     return board.currentEnemy();
//   }
//   if (board.checkDraw()) {
//     return Color::EMPTY;
//   }

//   uint8_t newMove = movePolicy(board);
//   board.makeMove(newMove);
//   Color val = playout(board, movePolicy);
//   auto encoding = board.encode();
//   {
//     std::lock_guard<std::mutex> lock(mapMutex);
//     myMap[encoding].visits++;
//     if (val == Color::BLACK) {
//       myMap[encoding].blackWins += 1;
//     } else if (val == Color::RED) {
//       myMap[encoding].redWins += 1; // Red wins should be incremented
//     }
//   }
//   board.undoMove();

//   return val;
// }

// void evaluateMove(C4Game board, uint8_t move, std::function<int(C4Game&)> movePolicy, int numSimulations, uint8_t& bestMove, optional<int>& maxVal, std::mutex& resultMutex) {
//   board.makeMove(move);
//   int total = 0;
//   for (int i = 0; i < numSimulations; i++) {
//     auto outcome = playout(board, movePolicy);
//     if (outcome == board.currentEnemy()) {
//       total += 1;
//     } else if (outcome == board.currentPlayer()) {
//       total -= 1;
//     }
//   }
//   board.undoMove();

//   std::lock_guard<std::mutex> lock(resultMutex);
//   if (!maxVal.has_value() || total > *maxVal) {
//     maxVal = total;
//     bestMove = move;
//   }
//   cout << +move << ": " << total << endl;
// }

// uint8_t treeSearch(C4Game& board, std::function<int(C4Game&)> movePolicy, int numSimulations = 10000) {
//   auto moveList = board.getMoves();
//   optional<int> maxVal = nullopt;
//   uint8_t bestMove = 0;
//   std::mutex resultMutex;

//   std::vector<std::thread> threads;
//   for (auto move : moveList) {
//     threads.emplace_back(evaluateMove, board, move, movePolicy, numSimulations, std::ref(bestMove), std::ref(maxVal), std::ref(resultMutex));
//   }

//   for (auto& thread : threads) {
//     thread.join();
//   }

//   return bestMove;
// }

int main() {
  srand(time(0));

  C4Game board;
  bool humanX, humanO;

  cout << "Is X a human? (1 = yes, 0 = no): ";
  cin >> humanX;
  cout << "Is O a human? (1 = yes, 0 = no): ";
  cin >> humanO;

  while (!board.gameOver()) {
    board.print();
  board.printBinary();

    Color currentPlayer = board.currentPlayer();
    cout << "Player " << +currentPlayer << "'s turn.\n";

    size_t move = 0;
    if ((currentPlayer == BLACK && humanX) || (currentPlayer == RED && humanO)) {
      cout << "Enter your move (0-" << C4Game::NUM_COLUMNS - 1 << "): ";
      cin >> move;
    } else {
      // // AI makes a move
      // if (currentPlayer == Color::BLACK) {
      //   move = treeSearch(board, UCBPolicy, 50000);
      // } else {
      //   move = treeSearch(board, randomPolicy, 50000);
      // }
      // cout << "AI chose move: " << move << endl;
    }

    if (board.makeMove(move)) {
      // Move successful
    } else {
      cout << "Invalid move. Try again.\n";
    }
  }

  board.print();
  board.printBinary();

  if (board.checkWin()) {
    cout << "Player " << +board.currentEnemy() << " wins!\n";
  } else {
    cout << "It's a draw!\n";
  }

  return 0;
}
