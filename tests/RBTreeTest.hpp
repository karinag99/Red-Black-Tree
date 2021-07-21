#ifndef _RBTREE_TEST_
#define _RBTREE_TEST_

#include "../Node.hpp"
#include "../RBTree.hpp"

template <class Type, class Allocator = MyAllocator<Node<Type>>>
class RBTreeTest : public RBTree<Type, Allocator>{
private:
    using RBTreeMemoryManager<Type, Allocator> :: root;
    using RBTreeMemoryManager<Type, Allocator> :: null_node;

public:
    using RBTreeFixupOperations<Type, Allocator> :: rotateLeft;
    using RBTreeFixupOperations<Type, Allocator> :: rotateRight;
    using RBTreeFixupOperations<Type, Allocator> :: transplant;

    using RBTreeMemoryManager<Type, Allocator> :: delete_not_null_nodes;

    Node<Type>* & get_root()
    {
        return  root;
    }

    Node<Type>* & get_null_node()
    {
        return null_node;
    }
};

using node_ptr = Node<int>*;
using tree = RBTreeTest<int>;
using tree_ref = RBTreeTest<int>&;

void init_tree(tree_ref red_black_tree)
{
    for(size_t i = 1; i <= 10; ++i)
        red_black_tree.insert(i);
}

bool are_equal_helper(node_ptr first_node, node_ptr second_node,
               node_ptr const& first_null_node, node_ptr const& second_null_node)
{
    if(first_node == first_null_node)
        return (second_node == second_null_node);

    return first_node->value == second_node->value
        && first_node->color == second_node->color
        && are_equal_helper(first_node->left, second_node->left, first_null_node, second_null_node)
        && are_equal_helper(first_node->right, second_node->right, first_null_node, second_null_node);
}

bool are_equal(tree_ref first, tree_ref second)
{
    return are_equal_helper(first.get_root(), second.get_root(), first.get_null_node(), second.get_null_node());
}

void init_tree_negative(tree_ref red_black_tree)
{
    for(int i = -1; i >= -10; --i)
        red_black_tree.insert(i);    
}

bool are_equal_nodes(node_ptr& first, node_ptr& second)
{
    return first->value == second->value
        && first->color == second->color;
}

bool have_same_parents(node_ptr& first, node_ptr& second)
{
    return first->parent->value == second->parent->value
        && first->parent->color == second->parent->color;
}

bool is_leaf(node_ptr& node, node_ptr& null_node)
{
    return node->left == null_node && node->right == null_node;
}

#endif