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

    /**
     * @brief performs a left rotation from given node - makes the given node the right child of its left child:
     * - the right child of the given node's left becomes the right child of the given node 
     * - the parents of the given node and its right child are swapped
     */
    void rotateLeft(node_ptr node)
    {
        node_ptr right_child = node->right; 

        node->right = right_child->left; 
        node->right->parent = node;

        transplant(node, right_child);
        
        node->parent = right_child;
        right_child->left = node;
    }

    /**
     * @brief performs a right rotation from given node - makes the given node the left child of its right child:
     * - the left child of the given node's right becomes the left child of the given node 
     * - the parents of the given node and its left child are swapped
     */
    void rotateRight(node_ptr node)
    {
        node_ptr left_child = node->left;

        node->left = left_child->right;
        node->left->parent = node;

        transplant(node, left_child);

        node->parent = left_child;
        left_child->right = node;
    }

    /**
     * @brief replaces the given tree as a child of its parent with a subtree 
     */
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
    /**
     * @brief Fixes the violated properties given the new node that may cause a violation
     * - after a red node is inserted property 2(requires the root to be black) 
     * and property 4(a red node cannot have a red child) might be violated
     * - if we fall in case 1 --the parent node and its sibling are red-- 
     *   then both of them are colored black 
     *   and their parent is colored red. So the node that may violates the properties is now their parent.
     * - if the parent is a left child:
     * - - if we fall in case 2 --the sibling of the parent is black and the violator is a right child--
     *     then we use a left rotation to turn this case into case 3
     * - - if we fall in case 3 --the sibling is black and the violator is also a left child--
     *     then the parent node is colored black and its parent - red, after a right rotation there are no longer
     *     two red nodes in a row(the fixup is done)
     * - if the parent is a right child - left should be swapped with right
     */
    void insert_fixup(node_ptr& violator)
    {
        node_ptr parent_node = violator->parent;
        node_ptr parents_sibling;

        while(parent_node->is_red())
        {
            parents_sibling = parent_node->is_left_child() ? parent_node->parent->right : 
                                                          parent_node->parent->left;

            if(parents_sibling->is_red()) //case 1
            {
               fix_parent_color(parents_sibling, parent_node);
               violator = violator->parent->parent;
               parent_node = violator->parent;
            }
            else
            {
                if(parent_node->is_left_child())
                {
                    ensure_violator_is_left_child(violator, parent_node); //case 2
                    make_parent_node_black(parent_node, rotateRight); //case 3
                }
                else
                {
                    ensure_violator_is_right_child(violator, parent_node); //case 2
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
    
    void ensure_violator_is_left_child(node_ptr& violator, node_ptr& parent_node)
    {
        if(violator == parent_node->right)
        {
            violator = parent_node;
            rotateLeft(violator);
            parent_node = violator->parent;
        }
    }

    void ensure_violator_is_right_child(node_ptr& violator, node_ptr& parent_node)
    {
        if(violator == parent_node->left)
        {
            violator = parent_node;
            rotateRight(violator);
            parent_node = violator->parent;
        }
    }

    void make_parent_node_black(node_ptr& parent_node, rotation_ptr rotation)
    {
        parent_node->make_black();
        parent_node->parent->make_red();
        (this->*rotation)(parent_node->parent);
    }
   
protected:
    /**
     * @brief Moves the extra black node up the tree
     * 
     * @param fixup_node - always points to a nonroot doubly black node
     */
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

    /**
     * @brief restores properties 1(every node is black or red), 2(the root is black) and 4(a red node cannot have a red child) 
     *        when the fixup node is a left child
     * - if we fall in case 1 --the sibling of the node is red--
     *   then the sibling is colored black and its parent - red
     *   after a left rotation the case is converted into case 2, 3, or 4
     * - if we fall in case 2 --the sibling is black and its children are also black--
     *   then the sibling is colored red
     * - if we fall in case 3 --sibling is black and its right child is also black(left child is red)--
     *   then we swap the colors of the sibling and the red child 
     *   a right rotation is performed and case 3 is converted into case 4
     * - if we fall in case 4 --sibling is black and its right child is red--
     *   then sibling is taking its parent color, the parent node and the red child are colored black
     *   after a left rotation the violating is fixed
     */
    void fixup_helper_left(node_ptr& fixup_node, node_ptr& sibling, node_ptr& fixup_parent)
    {
        if(sibling->is_red()) //case 1
        {
            make_sibling_black(sibling, fixup_parent, rotateLeft);
            sibling = fixup_parent->right;
        }

        if(sibling->left->is_black() && sibling->right->is_black()) //case 2
        {
            sibling->make_red();
            fixup_node = fixup_parent;
        }
        else
        {
            if(sibling->right->is_black()) //case 3
            {
                swap_colors(sibling, sibling->left, rotateRight);
                sibling = fixup_parent->right;
            }
            
            //case 4
            compensate_doubly_node(sibling, fixup_parent, sibling->right, rotateLeft);
            fixup_node = root;
        }
    }

    /**
     * @brief restores properties 1(every node is black or red), 2(the root is black) and 4(a red node cannot have a red child) 
     *        when the fixup node is a right child
     * - if we fall in case 1 --the sibling of the node is red--
     *   then the sibling is colored black and its parent - red
     *   after a right rotation the case is converted into case 2, 3, or 4
     * - if we fall in case 2 --the sibling is black and its children are also black--
     *   then the sibling is colored red
     * - if we fall in case 3 --sibling is black and its left child is also black(right child is red)--
     *   then we swap the colors of the sibling and the red child 
     *   a left rotation is performed and case 3 is converted into case 4
     * - if we fall in case 4 --sibling is black and its left child is red--
     *   then sibling is taking its parent color, the parent node and the red child are colored black
     *   after a right rotation the violating is fixed
     */
    void fixup_helper_right(node_ptr& fixup_node, node_ptr& sibling, node_ptr& fixup_parent)
    {
        if(sibling->is_red()) //case 1
        {
            make_sibling_black(sibling, fixup_parent, rotateRight);
            sibling = fixup_parent->left;
        }

        if(sibling->right->is_black() && sibling->left->is_black()) //case 2
        {
            sibling->make_red();
            fixup_node = fixup_parent;
        }
        else 
        {
            if(sibling->left->is_black()) //case 3
            {
                swap_colors(sibling, sibling->right, rotateLeft);
                sibling = fixup_parent->left;
            }

            // case 4
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
  
    void swap_colors(node_ptr& sibling, node_ptr& redChild, rotation_ptr rotation)
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