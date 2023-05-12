#include <iostream>
#include <string>
#include <vector>

using namespace std;

constexpr int EMPTY = 0; // -
constexpr int BLACK = 1; // X
constexpr int RED = 2; // O

size_t NUM_ROWS = 6;
size_t NUM_COLS = 7;

typedef vector < vector < size_t >> Board;

Board convertInputToBoard(const vector<string>& input) {
    Board board(NUM_ROWS, vector<size_t>(NUM_COLS, EMPTY));
    // Assuming the input format matches the printBoard format
    // Modify this conversion logic as needed
    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < NUM_COLS; ++j) {
            // cout << i << j << " " << input[i][2 * j + 1] << endl;
            if (input[i][2 * j + 1] == 'X') {
                board[i][j] = BLACK;
            } else if (input[i][2 * j + 1] == 'O') {
                board[i][j] = RED;
            }
        }
    }
    return board;
}

int getFirstEmptyRow(const Board & board, int col) {
  for (int row = NUM_ROWS - 1; row >=0; row--) {
    if (board[row][col] == EMPTY) {
      return row;
    }
  }
  return -1;
}

bool checkWin(const Board &board, int col) {
    int row = getFirstEmptyRow(board, col) + 1;
    int maxSquareSize = min(NUM_ROWS, NUM_COLS);
    cout << col << row << " ";
    int currentPiece = board[row][col];

    // go down the column
    for (int i = 1; i < maxSquareSize; i++) {
        int newRow = row + i;

        // check if found a matching piece i below the given piece
        if (newRow < NUM_ROWS && board[newRow][col] == currentPiece) {
            int leftCol = col - i;
            int rightCol = col + i;

            // check left square
            if (leftCol >= 0 && board[row][leftCol] == currentPiece && board[newRow][leftCol] == currentPiece) {
                return true;
            }

            // check right square
            if (rightCol < NUM_COLS && board[row][rightCol] == currentPiece && board[newRow][rightCol] == currentPiece) {
                return true;
            }
        }
    }

    return false;
}

void printBoard(const Board & board) {
  for (auto row: board) {
    cout << "|";
    for (auto space: row) {
      if (space == EMPTY) {
        cout << "-";
      } else if (space == BLACK) {
        cout << "X";
      } else if (space == RED) {
        cout << "O";
      }
      cout << "|";
    }
    cout << "\n";
  }

  cout << "|";
  for (int i = 0; i < NUM_COLS; i++) {
    cout << i << "|";
  }
  cout << '\n';
}

int main() {
    vector<string> inputBoard;
    string line;

    // Read the board from the standard input
    for(int i = 0; i < NUM_ROWS; i++){
        getline(cin, line);
        inputBoard.push_back(line);
    }

//     inputBoard = {
// "|-|-|-|-|-|-|-|",
// "|-|-|-|-|-|-|-|",
// "|-|-|-|-|X|-|-|",
// "|-|-|O|-|X|-|-|",
// "|-|X|O|X|O|-|-|",
// "|O|O|O|X|X|-|-|",
//     };

    // Convert the input to the Board type
    Board board = convertInputToBoard(inputBoard);

    printBoard(board);

    // Call checkWin for both players
    int col = -1;
    for(int i = 0; i < NUM_COLS; i++){
        if(checkWin(board, i)){
            col = i;
        }
    }

    if (col >=0 ) {
        cout << "win" << endl;
    } else {
        cout << "No one has won yet." << endl;
    }

    return 0;
}