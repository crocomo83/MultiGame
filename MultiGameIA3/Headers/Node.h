#pragma once
#include <vector>
#include <iostream>

class Node {

    public :
        enum Mode {
            Min, 
            Max
        };

    public:
        Node(float val, Mode mode_);

        void setMoveSymbol(std::string symbol);
        void addChild(Node* child);

        int getBestChild();
        void computeValue();
        void sortChildren();

        std::pair<int, float> findMinChild();
        std::pair<int, float> findMaxChild();

    public:
        float value;
        std::string moveSymbol;
        Mode mode;
        std::vector<Node*> children;
};

std::string modeToString(Node::Mode mode);