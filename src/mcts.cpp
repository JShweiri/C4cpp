#include "mcts.hpp"

Node::Node(C4Game state, Node* parent, uint8_t move)
    : state(state), parent(parent), visits(0), wins(0), move(move) {}

Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
}

bool Node::isFullyExpanded() const {
    return children.size() == state.getMoves().size();
}

bool Node::isLeaf() const {
    return children.empty();
}

double Node::getUCB1(double explorationParameter) const {
    if (visits == 0) return std::numeric_limits<double>::max();
    return wins / visits + explorationParameter * std::sqrt(std::log(parent->visits) / visits);
}

MCTS::MCTS(int iterations, SigmaZeroNet& model, torch::Device device)
    : iterations(iterations), model(model), device(device) {}

uint8_t MCTS::run(C4Game rootState) {
    Node* rootNode = new Node(rootState);

    for (int i = 0; i < iterations; ++i) {
        Node* promisingNode = selectPromisingNode(rootNode);
        if (!promisingNode || promisingNode->state.gameOver()) {
            continue;
        }
        expandNode(promisingNode);

        Node* nodeToExplore = promisingNode;
        if (!promisingNode->children.empty()) {
            nodeToExplore = promisingNode->children[rand() % promisingNode->children.size()];
        }

        if (!nodeToExplore) {
            std::cerr << "Error: nodeToExplore is null" << std::endl;
            continue;
        }

        double result = simulateRandomPlayout(nodeToExplore);
        backpropagate(nodeToExplore, result);
    }

    Node* bestNode = getBestChild(rootNode);
    if (!bestNode) {
        std::cerr << "Error: bestNode is null" << std::endl;
        delete rootNode;
        return 0;  // Or some error code
    }
    uint8_t bestMove = bestNode->move;
    delete rootNode;
    return bestMove;
}

Node* MCTS::selectPromisingNode(Node* rootNode) {
    Node* node = rootNode;
    while (!node->isLeaf()) {
        node = getBestNodeWithUCB1(node);
        if (!node) {
            std::cerr << "Error: getBestNodeWithUCB1 returned null" << std::endl;
            return nullptr;
        }
    }
    return node;
}

void MCTS::expandNode(Node* node) {
    std::vector<uint8_t> possibleMoves = node->state.getMoves();
    for (uint8_t move : possibleMoves) {
        C4Game newState = node->state;
        newState.makeMove(move);
        Node* newNode = new Node(newState, node, move);
        node->children.push_back(newNode);
    }
}

double MCTS::simulateWithNetwork(Node* node) {
    torch::NoGradGuard no_grad;
    torch::Tensor stateTensor = node->state.toTensor().to(device);
    auto [policy, value] = model.forward(stateTensor);
    return value.item<double>(); // Correct assignment
}

double MCTS::simulateRandomPlayout(Node* node) {
    C4Game tempState = node->state;
    while (!tempState.gameOver()) {
        std::vector<uint8_t> possibleMoves = tempState.getMoves();
        tempState.makeMove(possibleMoves[rand() % possibleMoves.size()]);
    }
    if (tempState.checkWin()) {
        if (tempState.currentPlayer() == node->state.currentEnemy()) {
            return 1.01;
        } else {
            return -1.0;
        }
    }
    return -0.01;
}

void MCTS::backpropagate(Node* node, double result) {
    Node* tempNode = node;
    while (tempNode != nullptr) {
        tempNode->visits++;
        tempNode->wins += result;
        tempNode = tempNode->parent;
    }
}

Node* MCTS::getBestChild(Node* node) const {
    if (!node) {
        std::cerr << "Error: node is null in getBestChild" << std::endl;
        return nullptr;
    }
    Node* bestNode = nullptr;
    double maxRatio = -std::numeric_limits<double>::max();
    for (Node* child : node->children) {
        if (!child) {
            std::cerr << "Error: child is null in getBestChild" << std::endl;
            continue;
        }
        double ratio = child->visits > 0 ? (child->wins / child->visits) : -std::numeric_limits<double>::max();
        if (ratio > maxRatio) {
            maxRatio = ratio;
            bestNode = child;
        }
    }
    return bestNode;
}

Node* MCTS::getBestNodeWithUCB1(Node* node) const {
    if (!node) {
        std::cerr << "Error: node is null in getBestNodeWithUCB1" << std::endl;
        return nullptr;
    }
    Node* bestNode = nullptr;
    double maxUCB1 = -std::numeric_limits<double>::max();
    for (Node* child : node->children) {
        if (!child) {
            std::cerr << "Error: child is null in getBestNodeWithUCB1" << std::endl;
            continue;
        }
        double ucb1 = child->getUCB1(1.2);
        if (ucb1 > maxUCB1) {
            maxUCB1 = ucb1;
            bestNode = child;
        }
    }
    return bestNode;
}
