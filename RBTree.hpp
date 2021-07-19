#ifndef _RED_BLACK_TREE_
#define _RED_BLACK_TREE_

#include "Node.hpp"
#include "MyAllocator.hpp"
#include "RBTreeMemoryManager.hpp"
#include "RBTreeFixupOperations.hpp"

template <class Type, class Allocator = MyAllocator<Node<Type>>>
class RBTree : public RBTreeFixupOperations<Type, Allocator>{
private:
    using node_ptr = Node<Type>*;

protected:
    using RBTreeMemoryManager<Type, Allocator> :: null_node;
    using RBTreeMemoryManager<Type, Allocator> :: root;
    using RBTreeMemoryManager<Type, Allocator> :: alloc;

    using RBTreeMemoryManager<Type, Allocator> :: delete_not_null_nodes;
    
    using RBTreeFixupOperations<Type, Allocator> :: transplant;
    using RBTreeFixupOperations<Type, Allocator> :: insert_fixup;
    using RBTreeFixupOperations<Type, Allocator> :: delete_fixup;

private:
    node_ptr find_node_with_value(const Type& value) const
    {
        node_ptr iter = root;

        while(iter != null_node && iter->value != value)
        {
            if(value < iter->value)
                iter = iter->left;
            else
                iter = iter->right;
        }

        return iter;
    }

    void calculate_height(node_ptr node, size_t& height, size_t curr_height = 0) const
    {
        if(node == null_node)
        {
            if(curr_height > height)
                height = curr_height;

            return;
        }

        calculate_height(node->left, height, curr_height + 1);
        calculate_height(node->right, height, curr_height + 1);
    }

// delete_node helper functions

    node_ptr get_successor(node_ptr node) const
    {
        while(node->left)
            node = node->left;

        return node;         
    }

    void swap_delete_node_with_successor(node_ptr& delete_helper, node_ptr& successor, 
                                         node_ptr& fixup_node, NodeColor& node_color)
    {
        std::swap(successor->value, delete_helper->value);
        fixup_node = successor->right;
        node_color = successor->color;
        delete_helper = successor;
    }

    void fix_right_child(node_ptr& delete_node, node_ptr& successor)
    {

        delete_node->right = successor->right;
        successor->right->parent = delete_node;
    }

//insert helper function

    node_ptr get_parent(const Type& value) const
    {
        node_ptr iter_parent = null_node;
        node_ptr iter = root;
    
        while(iter != null_node)
        {
            iter_parent = iter;

            if(value < iter->value)
                iter = iter->left;
            else if(value > iter->value)
                iter = iter->right;
            else    
                throw std::invalid_argument("Value already exists!");
        }

        return iter_parent;
    }

public:
    void insert(const Type& value)
    {
        node_ptr parent = get_parent(value);
        node_ptr new_node = alloc.allocate(value, parent, null_node);

        if(parent == null_node)
            root = new_node;
        else if(new_node->value < parent->value)
            parent->left = new_node;
        else    
            parent->right = new_node;

        insert_fixup(new_node);
    }

    void delete_node(const Type& value)
    {
        node_ptr delete_node = find_node_with_value(value);

        if(delete_node == null_node)
            throw std::invalid_argument("Value doesn't exist");

        NodeColor deleted_node_color = delete_node->color;
        node_ptr fixup_node;

        if(delete_node->left == null_node)
        {
            fixup_node = delete_node->right;
            transplant(delete_node, delete_node->right);
        }
        else if(delete_node->right == null_node)
        {
            fixup_node = delete_node->left;
            transplant(delete_node, delete_node->left);
        }
        else
        {
            node_ptr successor = get_successor(delete_node);
            swap_delete_node_with_successor(delete_node, successor, fixup_node, deleted_node_color);

            if(successor->parent == delete_node)
                fix_right_child(delete_node, successor);                                              
            else               
                transplant(successor, successor->right);               
        }

        alloc.deallocate(delete_node);  

        if (deleted_node_color == NodeColor :: Black)
            delete_fixup(fixup_node);
    }

    bool exists(const Type& value) const
    {
        return find_node_with_value(value) != null_node;
    }

    size_t black_height()const
    {
        node_ptr iter = root;
        size_t height = 0;

        while(iter != null_node)
        {
            if(iter->is_black())
                height++;
            
            iter = iter->left;
        }

        return height;
    }

    Allocator& get_allocator()
    {
        return alloc;
    }

    size_t height() const
    {
        size_t max_height = 0;

        calculate_height(root, max_height);

        return max_height;
    }

    bool empty() const
    {
        return root == null_node;
    }

    void clear()
    {
        delete_not_null_nodes(root);
        root = null_node;
    }
};

#endif