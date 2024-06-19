#include <torch/torch.h>
#include "board.hpp"
#include "mcts.hpp"
#include "sigmazero.hpp"

void train(SigmaZeroNet& model, torch::Device device, int numIterations, int numSelfPlayGames) {
    model.train();
    torch::optim::Adam optimizer(model.parameters(), torch::optim::AdamOptions(0.001));
    const int batchSize = 32;

    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::vector<std::pair<torch::Tensor, torch::Tensor>> trainingData;

        for (int i = 0; i < numSelfPlayGames; ++i) {
            C4Game game;
            MCTS mcts(1000, model, device);
            std::vector<std::pair<torch::Tensor, torch::Tensor>> gameData;
            while (!game.gameOver()) {
                torch::Tensor boardTensor = game.toTensor().to(device);
                auto [policy, value] = model.forward(boardTensor);
                torch::Tensor actionProbs = torch::softmax(policy, -1);
                gameData.push_back({boardTensor, actionProbs});

                uint8_t bestMove = mcts.run(game);
                game.makeMove(bestMove);
            }
            if (game.checkWin()) {
                double result = game.currentEnemy() == BLACK ? 1.0 : -1.0;
                for (auto& [state, actionProbs] : gameData) {
                    torch::Tensor valueTensor = torch::full({1}, result, device);
                    trainingData.push_back({state, valueTensor});
                    result = -result;  // Alternate between 1.0 and -1.0 for self-play
                }
            } else if (game.checkDraw()) {
                for (auto& [state, actionProbs] : gameData) {
                    torch::Tensor valueTensor = torch::zeros({1}, device);
                    trainingData.push_back({state, valueTensor});
                }
            }
        }

        for (size_t i = 0; i < trainingData.size(); i += batchSize) {
            auto batchEnd = std::min(i + batchSize, trainingData.size());
            std::vector<torch::Tensor> states, values;
            for (size_t j = i; j < batchEnd; ++j) {
                states.push_back(trainingData[j].first);
                values.push_back(trainingData[j].second);
            }

            torch::Tensor statesBatch = torch::stack(states).to(device);
            torch::Tensor valuesBatch = torch::stack(values).to(device);

            optimizer.zero_grad();
            auto [policy, value] = model.forward(statesBatch);
            torch::Tensor valueLoss = torch::mse_loss(value, valuesBatch);
            valueLoss.backward();
            optimizer.step();
        }

        std::cout << "Iteration " << iteration + 1 << " complete." << std::endl;
    }
}

int main() {
    torch::Device device(torch::kCPU);
    if (torch::cuda::is_available()) {
        device = torch::Device(torch::kCUDA);
    }

    SigmaZeroNet model;
    model.to(device);

    std::string input;
    std::cout << "Do you want to train the model? (yes/no): ";
    std::cin >> input;

    if (input == "yes") {
        int numIterations, numSelfPlayGames;
        std::cout << "Enter number of training iterations: ";
        std::cin >> numIterations;
        std::cout << "Enter number of self-play games per iteration: ";
        std::cin >> numSelfPlayGames;
        
        std::string checkpointPath;
        std::cout << "Enter model checkpoint file path to save: ";
        std::cin >> checkpointPath;

        train(model, device, numIterations, numSelfPlayGames);
        model.save_weights(checkpointPath);
    } else {
        std::string checkpointPath;
        std::cout << "Enter model checkpoint file path to load: ";
        std::cin >> checkpointPath;

        model.load_weights(checkpointPath);
    }

    C4Game game;
    MCTS mcts(1000, model, device);  // Set the number of iterations for MCTS

    game.print();

    while (!game.gameOver()) {
        if (game.currentPlayer() == BLACK) {
            std::cout << "Player 1's turn. Enter column (0-" << static_cast<int>(C4Game::NUM_COLUMNS - 1) << "): ";
            int column;
            std::cin >> column;
            if (!game.makeMove(column)) {
                std::cout << "Invalid move. Try again." << std::endl;
                continue;
            }
        } else {
            std::cout << "AI's turn." << std::endl;
            uint8_t bestMove = mcts.run(game);
            game.makeMove(bestMove);
            std::cout << "AI played column " << static_cast<int>(bestMove) << std::endl;
        }
        game.print();
    }

    if (game.checkWin()) {
        if (game.currentPlayer() == BLACK) {
            std::cout << "Player 2 (AI) wins!" << std::endl;
        } else {
            std::cout << "Player 1 wins!" << std::endl;
        }
    } else if (game.checkDraw()) {
        std::cout << "The game is a draw!" << std::endl;
    }

    return 0;
}
