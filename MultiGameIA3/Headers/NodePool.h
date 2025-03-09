#pragma once
#include <vector>
#include <iostream>
#include "Node.h"

class NodePool {

private:
    std::vector<Node> pool;  // Stocke tous les n�uds en m�moire contigu�
    size_t index = 0;  // Indice du prochain n�ud libre

public:
            NodePool();
            NodePool(size_t maxNodes);  // Pr�alloue de la m�moire

    Node*   createNode(float value, Node::Mode mode);
    void    reset();  // R�initialise le pool (sans free/realloc)
};