#include <iostream>
#include "board.hpp"
#include "mcts.hpp"

int main() {
    C4Game game;
    MCTS mcts(100000);  // Set the number of iterations for MCTS

    std::cout << "Welcome to Connect 4!" << std::endl;
    game.print();

    while (!game.gameOver()) {
        if (game.currentPlayer() == 1) {
            std::cout << "Player 1's turn. Enter column (0-" << static_cast<int>(C4Game::NUM_COLUMNS - 1) << "): ";
            // int column;
            // std::cin >> column;
            // if (!game.makeMove(column)) {
            //     std::cout << "Invalid move. Try again." << std::endl;
            //     continue;
            // }
            uint8_t bestMove = mcts.run(game);
                        game.makeMove(bestMove);

        } else {
            std::cout << "AI's turn." << std::endl;
            uint8_t bestMove = mcts.run(game);
            game.makeMove(bestMove);
            std::cout << "AI played column " << static_cast<int>(bestMove) << std::endl;
        }
        game.print();
    }

    if (game.checkWin()) {
        if (game.currentPlayer() == 1) {
            std::cout << "Player 2 (AI) wins!" << std::endl;
        } else {
            std::cout << "Player 1 wins!" << std::endl;
        }
    } else if (game.checkDraw()) {
        std::cout << "The game is a draw!" << std::endl;
    }

    return 0;
}
