#include "../Headers/NodePool.h"

NodePool::NodePool() {}

NodePool::NodePool(size_t maxNodes) {
    pool.reserve(maxNodes); 
}

Node* NodePool::createNode(float value, Node::Mode mode) {
    if (index >= pool.capacity()) {
        std::cout << "Pool allocator epuise : " << index << " >= " << pool.capacity() << std::endl;
        throw std::runtime_error("Pool allocator épuisé !");
    }
    pool.emplace_back(value, mode);  // Création du nœud dans le pool
    return &pool[index++];  // Retourne un pointeur vers le nœud créé
}

void NodePool::reset() { index = 0; }  // Réinitialise le pool