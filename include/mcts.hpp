#ifndef MCTS_HPP
#define MCTS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>
#include "board.hpp"

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
    MCTS(int iterations);
    uint8_t run(C4Game rootState);

private:
    int iterations;

    Node* selectPromisingNode(Node* rootNode);
    void expandNode(Node* node);
    double simulateRandomPlayout(Node* node);
    void backpropagate(Node* node, double result);
    Node* getBestChild(Node* node) const;
    Node* getBestNodeWithUCB1(Node* node) const;
};

#endif // MCTS_HPP
