#include "board.hpp"
#include <gtest/gtest.h>

class BoardTest : public ::testing::Test { 
};

TEST(BoardTest, Initialization) {
  Board board;
  ASSERT_EQ(board.NUM_ROWS, 6);
  ASSERT_EQ(board.NUM_COLUMNS, 7);
  ASSERT_EQ(board.currentPlayer(), Color::BLACK);
  ASSERT_EQ(board.currentEnemy(), Color::RED);
}

TEST(BoardTest, MakeMove) {
  Board board;
  ASSERT_TRUE(board.makeMove(0));
  ASSERT_EQ(board.currentPlayer(), Color::RED);
  ASSERT_TRUE(board.makeMove(0));
  ASSERT_EQ(board.currentPlayer(), Color::BLACK);
  ASSERT_FALSE(board.makeMove(7)); 
  ASSERT_FALSE(board.makeMove(-1)); 
}

TEST(BoardTest, MakeMove2) {
  Board board;
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  ASSERT_FALSE(board.makeMove(0)); 
}

TEST(BoardTest, UndoMove) {
  Board board;
  ASSERT_TRUE(board.makeMove(0)); 
  ASSERT_TRUE(board.undoMove(0)); 
  ASSERT_EQ(board.currentPlayer(), Color::BLACK); 
  ASSERT_FALSE(board.undoMove(0));
}

TEST(BoardTest, GetMoves) {
  Board board;
  std::vector<size_t> expectedMoves = {0, 1, 2, 3, 4, 5, 6};
  ASSERT_EQ(board.getMoves(), expectedMoves);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  board.makeMove(0);
  expectedMoves = {1, 2, 3, 4, 5, 6};
  ASSERT_EQ(board.getMoves(), expectedMoves);
}

TEST(BoardTest, CheckWin) {
  Board board;
  // 2x2
  board.makeMove(1);
  board.makeMove(3);
  board.makeMove(1);
  board.makeMove(5);
  board.makeMove(2);
  board.makeMove(6);
  board.makeMove(2);
  ASSERT_TRUE(board.checkWin());

  Board board2;
  // 3x3
  board2.makeMove(1);
  board2.makeMove(2);
  board2.makeMove(3);
  board2.makeMove(1);
  board2.makeMove(2);
  board2.makeMove(3);
  board2.makeMove(1);
  board2.makeMove(2);
  board2.makeMove(3);
  ASSERT_TRUE(board2.checkWin());

// Add more (maybe programatically)
}

TEST(BoardTest, CheckDraw) {
  Board board;
  for (size_t i = 0; i < board.NUM_ROWS; ++i) {
    for (size_t j = 0; j < board.NUM_COLUMNS; ++j) {
      if (i == board.NUM_ROWS - 1 && j == board.NUM_COLUMNS - 1) {
        break; 
      }
      board.makeMove(j);
    }
  }
  ASSERT_TRUE(board.checkDraw()); 
}

TEST(BoardTest, GameOver) {
  // Win condition
  Board board;
  board.makeMove(1);
  board.makeMove(3);
  board.makeMove(1);
  board.makeMove(5);
  board.makeMove(2);
  board.makeMove(6);
  board.makeMove(2);
  ASSERT_TRUE(board.gameOver());

  // Draw condition
  Board board2;
  for (size_t i = 0; i < board2.NUM_ROWS; ++i) {
    for (size_t j = 0; j < board2.NUM_COLUMNS; ++j) {
      board2.makeMove(j);
    }
  }
  ASSERT_TRUE(board2.gameOver());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}