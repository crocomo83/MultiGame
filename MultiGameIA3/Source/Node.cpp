#include "../Headers/Node.h"

#include <algorithm>
#include <iostream>

Node::Node(float val, Mode mode_) 
: value(val) 
, mode(mode_)
{

}

void Node::addChild(Node* child) {
    children.push_back(child);
}

void Node::setMoveSymbol(std::string symbol) {
    moveSymbol = symbol;
}

int Node::getBestChild() {
    if (mode == Node::Mode::Min) {
        return findMinChild().first;
    }
    else {
        return findMaxChild().first;
    }
}

void Node::computeValue() {
    if (mode == Node::Mode::Min) {
        value = findMinChild().second;
    }
    else {
        value = findMaxChild().second;
    }
}

std::pair<int, float> Node::findMinChild() {
    Node* minNode = children[0];
    int indexMin = 0;
    for (int i = 1; i < children.size(); i++) {
        Node* currentNode = children[i];
        if (currentNode < minNode) {
            currentNode = minNode;
            indexMin = i;
        }
    }
    return std::pair<int, float>(indexMin, minNode->value);
}

std::pair<int, float> Node::findMaxChild() {
    Node* maxNode = children[0];
    int indexMax = 0;
    for (int i = 1; i < children.size(); i++) {
        Node* currentNode = children[i];
        if (currentNode > maxNode) {
            currentNode = maxNode;
            indexMax = i;
        }
    }
    return std::pair<int, float>(indexMax, maxNode->value);
}

void Node::sortChildren() {
    if (children.size() == 0) {
        return;
    }

    for (int i = 0; i < children.size(); i++) {
        children[i]->sortChildren();
    }

    Node::Mode modeChildren = children[0]->mode;
    if (modeChildren == Node::Mode::Min) {
        std::sort(children.begin(), children.end(),
            [](Node* a, Node* b) {
                return a->value < b->value;
            });
    }
    else {
        std::sort(children.begin(), children.end(),
            [](Node* a, Node* b) {
                return a->value > b->value;
            });
    }
}

std::string modeToString(Node::Mode mode) {
    if (mode == Node::Mode::Min) {
        return "Min";
    }
    else {
        return "Max";
    }
}