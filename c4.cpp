#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <math.h>
#include <float.h>
#include <limits>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <cstdint>
using namespace std;

constexpr int EMPTY = 0; // -
constexpr int BLACK = 1; // X
constexpr int RED = 2;   // O

constexpr int NUM_THREADS = 7;

size_t NUM_ROWS = 6;
size_t NUM_COLS = 7;
int C = 800;
int NN = 100000;

typedef vector<vector<size_t>> Board;

void printBoard(const Board& board)
{
    for (auto row : board)
    {
        cout << "|";
        for (auto space : row)
        {
            if (space == EMPTY)
            {
                cout << "-";
            } else if (space == BLACK)
            {
                cout << "X";
            } else if (space == RED)
            {
                cout << "O";
            }
            cout << "|";
        }
        cout << "\n";
    }

    cout << "|";
    for (int i = 0; i < NUM_COLS; i++)
    {
        cout << i << "|";
    }
    cout << '\n';
}

int getFirstEmptyRow(const Board& board, int col)
{
    for (int row = NUM_ROWS - 1; row >= 0; --row)
    {
        if (board[row][col] == EMPTY)
        {
            return row;
        }
    }
    return -1;
}

vector<int> getAvailableMoves(const Board& board)
{
    vector<int> availableMoves;
    for (int col = 0; col < NUM_COLS; ++col)
    {
        if (getFirstEmptyRow(board, col) != -1)
        {
            availableMoves.push_back(col);
        }
    }
    return availableMoves;
}

int randomMove(const Board& board)
{
    auto v = getAvailableMoves(board);
    return v[rand() % v.size()];
}

void makeMove(Board& board, int col, int& player)
{
    int row = getFirstEmptyRow(board, col);
    if (row != -1)
    {
        board[row][col] = player;
        player = (player == BLACK ? RED : BLACK);
    }
}

void undoMove(Board& board, int col, int& player)
{
    for (int row = 0; row < NUM_ROWS; ++row)
    {
        if (board[row][col] != EMPTY)
        {
            board[row][col] = EMPTY;
            player = (player == BLACK ? RED : BLACK);
            break;
        }
    }
}

bool checkWin(const Board& board, int col)
{
    int row = getFirstEmptyRow(board, col) + 1;
    if (row >= NUM_ROWS)
        return false;
    int maxSquareSize = min(NUM_ROWS, NUM_COLS);
    int currentPiece = board[row][col];

    // go down the column
    for (int i = 1; i < maxSquareSize; i++)
    {
        int newRow = row + i;

        // check if found a matching piece i below the given piece
        if (newRow < NUM_ROWS && board[newRow][col] == currentPiece)
        {
            int leftCol = col - i;
            int rightCol = col + i;

            // check left square
            if (leftCol >= 0 && board[row][leftCol] == currentPiece && board[newRow][leftCol] == currentPiece)
            {
                return true;
            }

            // check right square
            if (rightCol < NUM_COLS && board[row][rightCol] == currentPiece && board[newRow][rightCol] == currentPiece)
            {
                return true;
            }
        }
    }

    return false;
}

bool checkDraw(const Board& board)
{
    for (int i = 0; i < NUM_COLS; i++)
    {
        if (board[0][i] == EMPTY)
        {
            return false;
        }
    }
    return true;
}

// BLACK + X
// RED - O

int randomPlayout(Board& board, int& player, int col)
{
    if (checkWin(board, col))
    {
        int row = getFirstEmptyRow(board, col) + 1;
        int winningPiece = board[row][col];
        // cout << "winning piece: " << winningPiece << endl;
        // printBoard(board);
        return ((winningPiece == BLACK) ? 1 : -1);
    }
    if (checkDraw(board))
    {
        return 0;
    }

    int newMove = randomMove(board);
    makeMove(board, newMove, player);
    int val = randomPlayout(board, player, newMove);
    undoMove(board, newMove, player);

    return val;
}


int monteCarloMove(Board& board, int& player)
{
    auto moveList = getAvailableMoves(board);

    int maxVal = -NN - 1;
    int bestMove = -1;

    for (auto move : moveList)
    {
        makeMove(board, move, player);
        int total = 0;
        for (int i = 0; i < NN; i++)
        {
            total += randomPlayout(board, player, move);
        }
        undoMove(board, move, player);

        if (player == RED)
            total = -total;
        cout << total << endl;

        if (total > maxVal)
        {
            maxVal = total;
            bestMove = move;
        }
    }
    return bestMove;
}

int main(int argc, char** argv)
{
    if (argc >= 3)
    {
        NUM_ROWS = atoi(argv[1]);
        NUM_COLS = atoi(argv[2]);
    }
    if (argc >= 4)
    {
        NN = atoi(argv[3]);
    }
    if (argc >= 5)
    {
        C = atoi(argv[4]);
    }

    srand(time(0));

    Board board(NUM_ROWS, vector<size_t>(NUM_COLS, EMPTY));
    int currentPlayer = BLACK;
    bool gameInProgress = true;
    bool humanX, humanO;

    cout << "Is X a human? (1 = yes, 0 = no): ";
    cin >> humanX;
    cout << "Is O a human? (1 = yes, 0 = no): ";
    cin >> humanO;

    printBoard(board);

    while (gameInProgress)
    {
        int col;

        if ((currentPlayer == BLACK && humanX) || (currentPlayer == RED && humanO))
        {
            cout << "Player " << (currentPlayer == BLACK ? "X" : "O") << " to move. Choose a column (0-" << NUM_COLS - 1 << "): ";
            cin >> col;
        } else
        {
            col = (currentPlayer == BLACK ? monteCarloMove(board, currentPlayer) : monteCarloMove(board, currentPlayer));
            cout << "CPU plays: " << col << endl;
        }

        if (col < 0 || col >= NUM_COLS || getFirstEmptyRow(board, col) == -1)
        {
            cout << "Invalid move. Please try again." << endl;
            continue;
        }

        makeMove(board, col, currentPlayer);
        printBoard(board);

        if (checkWin(board, col))
        {
            gameInProgress = false;
            cout << "Player " << (currentPlayer == BLACK ? "O" : "X") << " wins!" << endl;
        } else if (checkDraw(board))
        {
            gameInProgress = false;
            cout << "It's a draw!" << endl;
        }
    }

    return 0;
}