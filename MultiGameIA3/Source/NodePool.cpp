#include "../Headers/NodePool.h"

NodePool::NodePool() {}

NodePool::NodePool(size_t maxNodes) {
    pool.reserve(maxNodes); 
}

Node* NodePool::createNode(float value, Node::Mode mode) {
    if (index >= pool.capacity()) {
        std::cout << "Pool allocator epuise : " << index << " >= " << pool.capacity() << std::endl;
        throw std::runtime_error("Pool allocator �puis� !");
    }
    pool.emplace_back(value, mode);  // Cr�ation du n�ud dans le pool
    return &pool[index++];  // Retourne un pointeur vers le n�ud cr��
}

void NodePool::reset() { index = 0; }  // R�initialise le pool