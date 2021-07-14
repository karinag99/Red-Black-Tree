#ifndef _RBTREE_NODE_
#define _RBTREE_NODE_

enum class NodeColor : bool {Black, Red}; 

template <class Type>
struct Node{ 
public:
    using node_ptr = Node<Type>*;

    Type  value;
    Node *parent;
    Node *left, *right;
    NodeColor color;

public:
    Node()
        : left(nullptr)
        , right(nullptr)
        , color(NodeColor :: Black) 
    { }

    Node(const Type& value, node_ptr parent, node_ptr null_node) 
        : value(value)
        , parent(parent)
        , left(null_node)
        , right(null_node)
        , color(NodeColor :: Red)
    { }

    Node(const node_ptr& other) 
        : value(other->value)
        , left(nullptr)
        , right(nullptr)
        , color(other->color)
    { }

    /**
     * @brief if parent exists returns whether the node is left child otherwise return false
     */
    bool is_left_child() const
    {
        return parent ? this == parent->left : false;
    }

    bool is_black() const
    {
        return color == NodeColor :: Black;
    }

    bool is_red() const
    {
        return !is_black();
    }

    void make_black()
    {
        color = NodeColor :: Black;
    }

    void make_red()
    {
        color = NodeColor :: Red;
    }
};

#endif