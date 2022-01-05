#include <bits/stdc++.h>
using namespace std;

//TODO: convert to bitboard
// this means change the move undo and hash to bitboard
//save to binary file?
//faster way of saving/loading?

int NUM_ROWS = 6;
int NUM_COLS = 7;
int C = 1;
int NN = 1000;

#define EMPTY 0 // -
#define BLACK 1 // X
#define RED 2 // O

unordered_map<string, pair<int, double>> cache; // hashedBoard -> timesVisited, score

void printCache(unordered_map<string, pair<int, double>> cache){
	for(auto it = cache.cbegin(); it != cache.cend(); ++it){
		cout << it->first << " " << it->second.first << " " << it->second.second << "\n";
	}
}

void saveCache(){
    string filename = "cache";
    stringstream ss;
    ss << filename << char(NUM_ROWS + 48) << char(NUM_COLS + 48) << ".txt";
    ss >> filename;
    cout << "saving: " << filename << '\n';
ofstream fout(filename);
	for(auto it = cache.cbegin(); it != cache.cend(); ++it){
		fout << it->first << " " << it->second.first << " " << it->second.second << "\n";
	}
	fout.close();
}

void loadCache(){
	string filename = "cache";
    stringstream ss;
    ss << filename << char(NUM_ROWS + 48) << char(NUM_COLS + 48) << ".txt";
    ss >> filename;
    cout << "loading: " << filename << '\n';
	ifstream fin(filename);
	string line;
	while(getline(fin, line)){
		stringstream ss(line);
		string board;
		int timesVisited;
		double score;
		ss >> board >> timesVisited >> score;
		cache[board] = make_pair(timesVisited, score);
	}
	fin.close();
}

vector<vector<int>> board;

string hashBoard(vector<vector<int>> board){
	string hash = "";
	for(int i = 0; i < NUM_ROWS; i++){
		for(int j = 0; j < NUM_COLS; j++){
			hash += to_string(board[i][j]);
		}
	}
	return hash;
}

void printBoard(vector<vector<int>> board){
    for(auto row : board){
        cout << "|";
        for(auto space : row){
            if(space == EMPTY){
                cout << "-";
            } else if(space == BLACK){
                cout << "X";
            } else if(space == RED){
                cout << "O";
            }
            cout << "|";
        }
        cout << "\n";
    }
    
    cout << "|";
    for(int i = 0; i < NUM_COLS; i++){
        cout << i << "|";
    }
    cout << '\n';
}

int getFirstEmptyRow(vector<vector<int>> &board, int col){
    for(int i = NUM_ROWS-1; i >= 0; i--){
        if(board[i][col] == EMPTY){
            return i;
        }
    }
    return -1;
}

void makeMove(vector<vector<int>> &board, int row, int col, int &player){
    board[row][col] = player;
    player = (player == BLACK ? RED : BLACK);
}

void makeMove(vector<vector<int>> &board, int col, int &player){
    int row = getFirstEmptyRow(board, col);
    if(row < 0){ cout << "ERROR: COL FULL";}
    makeMove(board, row, col, player);
}

void undoMove(vector<vector<int>> &board, int row, int col, int &player){
    board[row][col] = EMPTY;
    player = (player == BLACK ? RED : BLACK);
}

void undoMove(vector<vector<int>> &board, int col, int &player){
    int row = getFirstEmptyRow(board, col) + 1;
    undoMove(board, row, col, player);
}

bool checkWin(vector<vector<int>> &board, int player){
    int maxSquareSize = min(NUM_ROWS, NUM_COLS);
    
    for (int x = 0; x < NUM_COLS; x++) {
        for (int y = 0; y < NUM_ROWS; y++) {
            for (int n = 1; n < maxSquareSize; n++) {
                if (board[y][x] == player && ((y + n) < NUM_ROWS) && ((x + n) < NUM_COLS)) {
                    if (board[y][x] == player && board[y][x] == board[y + n][x] && board[y][x] == board[y][x + n] && board[y][x] == board[y + n][x + n]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool checkDraw(vector<vector<int>> &board){
    for(auto row : board){
        for(auto space : row){
            if(space == EMPTY){
                return false;
            }
        }
    }
    return true;
}

bool gameOver(vector<vector<int>> &board){
    return checkDraw(board) || checkWin(board, BLACK) || checkWin(board, RED);
}

vector<int> getAvailableMoves(vector<vector<int>> &board){
    vector<int> ret;
    for(int i = 0; i < NUM_COLS; i++){
        if(getFirstEmptyRow(board, i) != -1){
            ret.push_back(i);
        }
    }
    return ret;
}

int randomMove(vector<vector<int>> &board){
    auto v = getAvailableMoves(board);
    return v[rand()%v.size()];
}

double randomPlayout(vector<vector<int>> &board, int &player){
    if(checkWin(board, BLACK) || checkWin(board, RED)){
      cache[hashBoard(board)].first++;
      cache[hashBoard(board)].second += -1;
      return -1;
    }
    if(checkDraw(board)){
		cache[hashBoard(board)].first++;
      cache[hashBoard(board)].second += 0;
        return 0;
    }

	auto moveList = getAvailableMoves(board);
    
    double maxScore = -DBL_MAX;
    vector<int> bestMove;

    int N;
    int ni;
    int vi;

    for (auto move : moveList) {
	  N = cache[hashBoard(board)].first;
	  if(N == 0){
            cache[hashBoard(board)].first = 1;
            N = 1;
      }

      makeMove(board, move, player);

	  ni = cache[hashBoard(board)].first;
	  if(ni == 0){
            cache[hashBoard(board)].first = 1;
            ni = 1;
      }

	  vi = cache[hashBoard(board)].second;
      double score = double(vi / ni) + C * sqrt(log(N) / ni);

	  	//   cout << "N: " << N << " ni: " << ni << " vi: " << vi << " score: " << score << endl;

      undoMove(board, move, player);
      if (score >= maxScore) {
		if(score == maxScore){
			bestMove.push_back(move);
		} else {
			bestMove.clear();
			bestMove.push_back(move);
		}
        maxScore = score;
      }
    }

	int move = bestMove[rand()%bestMove.size()];

    makeMove(board, move, player);
    double val = -randomPlayout(board, player);
    undoMove(board, move, player);
	cache[hashBoard(board)].first++;
	cache[hashBoard(board)].second += val;

	// printBoard(board);
    // cout << val << "\n\n";

        return val;
}

int monteCarloMove(vector<vector<int>> &board, int player){
    auto moveList = getAvailableMoves(board);
    
    double maxVal = -NN -1;
    int bestMove = -1;
    
    for(auto move : moveList){
        makeMove(board, move, player);
        double total = 0;
        for(int i = 0; i < NN; i++){
            total -= randomPlayout(board, player);
        }
        total = total/NN;
        undoMove(board, move, player);
        cout << "player: " << player << " move: " << move << " valueOfMove: " << total <<"\n";
        if(total > maxVal){
            maxVal = total;
            bestMove = move;
        }
    }
    saveCache();
    return bestMove;
}

int main(int argc, char** argv)
{
//   cout << argc << endl;
  if (argc >= 3) {
    NUM_ROWS = argv[1] == NULL ? NUM_ROWS : atoi(argv[1]);
    NUM_COLS = argv[2] == NULL ? NUM_COLS : atoi(argv[2]);
	}
	if(argc >= 4){
    	NN = argv[3] == NULL ? NN : atoi(argv[3]);
	}

	loadCache();
	//   printCache(cache);

	board = vector<vector<int>>(NUM_ROWS, vector<int>(NUM_COLS, EMPTY));	

    srand (time(NULL));
    
    int player = BLACK;
    
    printBoard(board);
        cout << "\n";
    
    while(!gameOver(board)){
        
        if(player == BLACK){
                    int move;
        cout << "Enter Column: ";
        cin >> move;
            makeMove(board, move, player);

		    // int move = monteCarloMove(board, player);
            // makeMove(board, move, player);
        }

		if(gameOver(board)){
			break;
		}
        
        if(player == RED){
            int move = monteCarloMove(board, player);
            // int move = randomMove(board);
            makeMove(board, move, player);
        }
        
        printBoard(board);
        cout << "\n";

	// cout << "\n\n";
    // printCache(cache);
    }
    
    if(player == BLACK){
    cout << "Game Over! X Wins!";
    } else {
    cout << "Game Over! O Wins!";
    }

}
