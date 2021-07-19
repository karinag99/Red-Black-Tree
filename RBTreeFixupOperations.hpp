#ifndef _RBTREE_FIXUP_
#define _RBTREE_FIXUP_

#include "RBTreeMemoryManager.hpp"

template <class Type, class Allocator = MyAllocator<Node<Type>>>
class RBTreeFixupOperations : public RBTreeMemoryManager<Type, Allocator>{
private:
    using node_ptr      = Node<Type>*;
    using rotation_ptr  = void(RBTreeFixupOperations::*)(node_ptr);
    
protected:
    using RBTreeMemoryManager<Type> :: null_node;
    using RBTreeMemoryManager<Type> :: root;

    void rotateLeft(node_ptr node)
    {
        node_ptr right_child = node->right; 

        node->right = right_child->left; 
        node->right->parent = node;

        transplant(node, right_child);
        
        node->parent = right_child;
        right_child->left = node;
    }

    void rotateRight(node_ptr node)
    {
        node_ptr left_child = node->left;

        node->left = left_child->right;
        node->left->parent = node;

        transplant(node, left_child);

        node->parent = left_child;
        left_child->right = node;
    }

    void transplant(node_ptr& tree, node_ptr& subtree)
    {
        if(tree->parent == null_node)
            root = subtree;
        else if(tree->is_left_child())
            tree->parent->left = subtree;
        else
            tree->parent->right = subtree;

            subtree->parent = tree->parent;
    }

protected:
    void insert_fixup(node_ptr& new_node)
    {
        node_ptr parent_node = new_node->parent;
        node_ptr parents_sibling;

        while(parent_node->is_red())
        {
            parents_sibling = parent_node->is_left_child() ? parent_node->parent->right : 
                                                          parent_node->parent->left;

            //case 1
            if(parents_sibling->is_red())
            {
               fix_parent_color(parents_sibling, parent_node);
               new_node = new_node->parent->parent;
               parent_node = new_node->parent;
            }
            else
            {
                if(parent_node->is_left_child())
                {
                    ensure_new_node_is_left_child(new_node, parent_node); //case 2
                    make_parent_node_black(parent_node, rotateRight); //case 3
                }
                else
                {
                    ensure_new_node_is_right_child(new_node, parent_node); //case 2
                    make_parent_node_black(parent_node, rotateLeft); // case 3
                }
            }
        }

        root->make_black();
    }

private:
//insert_fixup helper functions

    void fix_parent_color(node_ptr& parents_sibling, node_ptr& parent_node)
    {
        parents_sibling->make_black();
        parent_node->make_black();
        parent_node->parent->make_red();
    }
    
    void ensure_new_node_is_left_child(node_ptr& new_node, node_ptr& parent_node)
    {
        if(new_node == parent_node->right)
        {
            new_node = parent_node;
            rotateLeft(new_node);
            parent_node = new_node->parent;
        }
    }

    void ensure_new_node_is_right_child(node_ptr& new_node, node_ptr& parent_node)
    {
        if(new_node == parent_node->left)
        {
            new_node = parent_node;
            rotateRight(new_node);
            parent_node = new_node->parent;
        }
    }

    void make_parent_node_black(node_ptr& parent_node, rotation_ptr rotation)
    {
        parent_node->make_black();
        parent_node->parent->make_red();
        (this->*rotation)(parent_node->parent);
    }
   
protected:
    void delete_fixup(node_ptr fixup_node)
    {
        node_ptr fixup_parent;
        node_ptr sibling;

        while(root != fixup_node && fixup_node->is_black())
        {
            fixup_parent = fixup_node->parent;

            if(fixup_node->is_left_child())
            {
                sibling = fixup_parent->right;
                fixup_helper_left(fixup_node, sibling, fixup_parent);
            } else
            {
                sibling = fixup_parent->left;
                fixup_helper_right(fixup_node, sibling, fixup_parent);
            }
        }

        fixup_node->make_black();
    }

private:
//delete_fixup helper functions

    void fixup_helper_left(node_ptr& fixup_node, node_ptr& sibling, node_ptr& fixup_parent)
    {
        if(sibling->is_red())
        {
            make_sibling_black(sibling, fixup_parent, rotateLeft);
            sibling = fixup_parent->right;
        }

        if(sibling->left->is_black() && sibling->right->is_black())
        {
            sibling->make_red();
            fixup_node = fixup_parent;
        }
        else 
        {
            if(sibling->right->is_black())
            {
                swap_childrens_colors(sibling, sibling->left, rotateRight);
                sibling = fixup_parent->right;
            }
            
            compensate_doubly_node(sibling, fixup_parent, sibling->right, rotateLeft);
            fixup_node = root;
        }
    }

    void fixup_helper_right(node_ptr& fixup_node, node_ptr& sibling, node_ptr& fixup_parent)
    {
        if(sibling->is_red())
        {
            make_sibling_black(sibling, fixup_parent, rotateRight);
            sibling = fixup_parent->left;
        }

        if(sibling->right->is_black() && sibling->left->is_black())
        {
            sibling->make_red();
            fixup_node = fixup_parent;
        }
        else 
        {
            if(sibling->left->is_black())
            {
                swap_childrens_colors(sibling, sibling->right, rotateLeft);
                sibling - fixup_parent->left;
            }

            compensate_doubly_node(sibling, fixup_parent, sibling->left, rotateRight);
            fixup_node = root;
        }
    }

    void make_sibling_black(node_ptr& sibling, node_ptr& parent_node, rotation_ptr rotation)
    {
        sibling->make_black();
        parent_node->make_red();
        (this->*rotation)(parent_node);
    }
  
    void swap_childrens_colors(node_ptr& sibling, node_ptr& redChild, rotation_ptr rotation)
    {
        redChild->make_black();
        sibling->make_red();
        (this->*rotation)(sibling);
    }

    void compensate_doubly_node(node_ptr& sibling, node_ptr& parent_node, node_ptr& redChild, rotation_ptr rotation)
        {
            sibling->color = parent_node->color;
            parent_node->make_black();
            redChild->make_black();
            (this->*rotation)(parent_node);
        }

};
#endif