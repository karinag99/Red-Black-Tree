#ifndef RBTREE_MEMORY_MANAGER
#define RBTREE_MEMORY_MANAGER

#include "Node.hpp"
#include "MyAllocator.hpp"

template <class Type, class Allocator = MyAllocator<Node<Type>>>
class RBTreeMemoryManager{
protected:
    using node_ptr = Node<Type>*;

    node_ptr root;
    node_ptr null_node;
    MyAllocator<Node<Type>> alloc;

    void delete_not_null_nodes(node_ptr node)
    {
        if(node == null_node)
            return;

        delete_not_null_nodes(node->left);
        delete_not_null_nodes(node->right);

        alloc.deallocate(node);
    }

private:
    node_ptr copy_helper(node_ptr parent, node_ptr other_node, const node_ptr& other_null_node)
    {
        if(other_node == other_null_node)
            return null_node;

        node_ptr current = alloc.allocate(other_node);
        current->parent = parent;

        current->left = copy_helper(current, other_node->left, other_null_node);
        current->right = copy_helper(current, other_node->right, other_null_node);

        return current;
    }

    void copy(const RBTreeMemoryManager& other)
    {
        null_node = alloc.allocate();

        root = copy_helper(null_node, other.root, other.null_node);
    }

    void delete_all_nodes()
    {
        delete_not_null_nodes(root);
        alloc.deallocate(null_node);
    }

public: 
    RBTreeMemoryManager()
    {
        null_node = alloc.allocate();
        root = null_node;
    }

    RBTreeMemoryManager(const RBTreeMemoryManager<Type, Allocator>& other) : alloc()
    {
        copy(other);
    }

    RBTreeMemoryManager& operator=(const RBTreeMemoryManager<Type, Allocator>& other)
    {
        if(this != &other)
        {
            delete_all_nodes();
            copy(other);
        }

        return *this;
    }

    ~RBTreeMemoryManager()
    {
        delete_all_nodes();
    }
};

#endif