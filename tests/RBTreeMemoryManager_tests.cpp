#include "../catch.hpp"
#include "../RBTreeMemoryManager.hpp"
#include "../Red-black_tree.cpp"

template <class Type, class Allocator = MyAllocator<Node<Type>>>
class MemoryManagerTest : public RBTree<Type, Allocator>{
private:
    using RBTreeMemoryManager<Type, Allocator> :: root;
    using RBTreeMemoryManager<Type, Allocator> :: null_node;

public:
    Node<Type>* get_root()
    {
        return  root;
    }

    Node<Type>* get_null_node()
    {
        return null_node;
    }
};

using node_ptr = Node<int>*;
using tree = MemoryManagerTest<int>;
using tree_ref = MemoryManagerTest<int>&;

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


SCENARIO("Testing default constructor")
{
    GIVEN("An empty tree")
    {
        RBTree<int> empty_tree;

        THEN("Black-size should be 1")
        {
            REQUIRE(empty_tree.black_height() == 0);
        }
        
        THEN("The size of the allocated nodes must be 1")
        {
            REQUIRE(empty_tree.get_allocator().size() == 1);
        }

        THEN("Height should be 0")
        {
            REQUIRE(empty_tree.height() == 0);
        } 

        THEN("Empty shpukd return true")
        {
            CHECK(empty_tree.empty());
        }       
    }
}

SCENARIO("Testing copy constructor")
{
    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        WHEN("A copy is constrcuted")
        {
            tree copy_test(test);

            THEN("The black-size of both trees should be equal")
            {
                REQUIRE(test.black_height() == copy_test.black_height());
            }

            THEN("The height of both trees should be equal")
            {
                REQUIRE(test.height() == copy_test.height());
            }

            THEN("Equal number of nodes shpuld be allocated")
            {
                REQUIRE(test.get_allocator().size() == copy_test.get_allocator().size());
            }

            THEN("Trees should be equal")
            {
                CHECK(are_equal(test, copy_test));
            }
        }
    }

    GIVEN("An empty array")
    {
        tree empty;

        WHEN("A copy is constructed")
        {
            tree copy_empty(empty);

            THEN("The black size of the new tree should be 0")
            {
                REQUIRE(copy_empty.black_height() == 0);
            }

            THEN("The height of the new tree should be 0")
            {
                REQUIRE(copy_empty.height() == 0);
            }

            THEN("The number of allocated nodes should be 1")
            {
                REQUIRE(copy_empty.get_allocator().size() == 1);
            }

            THEN("Empty should return true")
            {
                CHECK(copy_empty.empty());
            }
        }
    }
}

SCENARIO("Testing assignment operator")
{
    GIVEN("Two non-empty trees")
    {
        tree tree1, tree2;

        init_tree(tree1);
        tree copy_tree1(tree1);

        init_tree_negative(tree2);

        WHEN("Trees are equalized")
        {
            tree2 = tree1;

            THEN("The tree on the right hand side shouldn't change")
            {
                CHECK(are_equal(tree1, copy_tree1));
            }
        
            THEN("Both trees should have same black-height")
            {
                CHECK(tree1.black_height() == tree2.black_height());
            }

            THEN("Both trees should have same height")
            {
                CHECK(tree1.height() == tree2.height());
            }

            THEN("Trees should be equal")
            {
                CHECK(are_equal(tree1, tree2));
            }
        
            THEN("The size of the allocated nodes of both trees should be equal")
            {
                REQUIRE(tree1.get_allocator().size() == tree2.get_allocator().size());
            }
        }

        WHEN("A tree is equalized to itself")
        {
            tree1 = tree1;

            THEN("The tree shouldn't change")
            {
                CHECK(are_equal(tree1, copy_tree1));
            }
        }
    }
}

