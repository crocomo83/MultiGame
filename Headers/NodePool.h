#pragma once
#include <vector>
#include <iostream>
#include "Node.h"

class NodePool {

private:
    std::vector<Node> pool;  // Stocke tous les nœuds en mémoire contiguë
    size_t index = 0;  // Indice du prochain nœud libre

public:
            NodePool();
            NodePool(size_t maxNodes);  // Préalloue de la mémoire

    Node*   createNode(float value, Node::Mode mode);
    void    reset();  // Réinitialise le pool (sans free/realloc)
};