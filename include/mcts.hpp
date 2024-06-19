#ifndef MCTS_HPP
#define MCTS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>
#include "board.hpp"
#include "sigmazero.hpp"

class Node {
public:
    C4Game state;
    Node* parent;
    std::vector<Node*> children;
    int visits;
    double wins;
    uint8_t move;

    Node(C4Game state, Node* parent = nullptr, uint8_t move = 0);
    ~Node();

    bool isFullyExpanded() const;
    bool isLeaf() const;
    double getUCB1(double explorationParameter = std::sqrt(2)) const;
};

class MCTS {
public:
    MCTS(int iterations, SigmaZeroNet& model, torch::Device device);
    uint8_t run(C4Game rootState);

private:
    Node* selectPromisingNode(Node* rootNode);
    void expandNode(Node* node);
    double simulateWithNetwork(Node* node);
    double simulateRandomPlayout(Node* node); // Declare this method here
    void backpropagate(Node* node, double result);
    Node* getBestChild(Node* node) const;
    Node* getBestNodeWithUCB1(Node* node) const;

    int iterations;
    SigmaZeroNet& model;
    torch::Device device;
};

#endif // MCTS_HPP
