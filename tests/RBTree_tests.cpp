#include "catch.hpp"
#include "RBTreeTest.hpp"

SCENARIO("Testing insert function")
{
    GIVEN("An empty tree")
    {
        tree test;

        WHEN("A new element is inserted")
        {
            test.insert(10);

            THEN("Empty function should return false")
            {
                CHECK_FALSE(test.empty());
            }

            THEN("The new element should exist in the tree")
            {
                CHECK(test.exists(10));
            }

            THEN("The root should contain the new element")
            {
                REQUIRE(test.get_root()->value == 10);
            }

            THEN("The root should be black")
            {
                CHECK(test.get_root()->is_black());
            }

            THEN("The black height should be 1")
            {
                REQUIRE(test.black_height() == 1);
            }

            THEN("The height should be 1")
            {
                REQUIRE(test.height() == 1);
            }

            THEN("The size of the allocator should be 2")
            {
                REQUIRE(test.get_allocator().size() == 2);
            }
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        WHEN("An existing value is inserted")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(test.insert(10), std::invalid_argument);
            }
        }

        WHEN("A new element is inserted")
        {
            test.insert(0);

            THEN("The new element should exist in the tree")
            {
                CHECK(test.exists(10));
            }

            THEN("The height and the black height shouldn't change")
            {
                REQUIRE(test.black_height() == 3);
                REQUIRE(test.height() == 5);
            }

            THEN("The size of the allocator should increase")
            {
                REQUIRE(test.get_allocator().size() == 12);
            }

            THEN("The new node should be a left child")
            {
                REQUIRE(test.get_root()->left->left->left->value == 0);
            }

            THEN("The new node should be a red leaf that contains the new value")
            {
                CHECK(test.get_root()->left->left->left->is_red());
                CHECK(is_leaf(test.get_root()->left->left->left, test.get_null_node()));

                REQUIRE(test.get_root()->left->left->left->value == 0);
            }

            THEN("The parent of the new node should be valid")
            {
                REQUIRE(test.get_root()->left->left->left->parent->value == 1);
            }
        }
    }
}

SCENARIO("Testing erase function")
{
    GIVEN("An empty tree")
    {
        tree test;

        WHEN("An element is erased")
        {
            THEN("An exception should be trown")
            {
                REQUIRE_THROWS_AS(test.erase(0), std::invalid_argument);
            }
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        tree copy(test);

        WHEN("An element that doesn't exist is deleted")
        {
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(test.erase(11), std::invalid_argument);
            }
        }
    
        WHEN("An element that exists with no children is deleted")
        {
            test.erase(10);

            THEN("The element shoudn't exist")
            {
                CHECK_FALSE(test.exists(10));
            }

            THEN("Deleted node should be replaced with null node")
            {
                REQUIRE(test.get_root()->right->right->right->right == test.get_null_node());
            }

            THEN("The size of the allocator should decrease")
            {
                REQUIRE(test.get_allocator().size() == 10);
            }
       }

        WHEN("An element that exists with only one child(right child) is deleted")
        {
            test.erase(9);

            THEN("The element shouldn't exist")
            {
                CHECK_FALSE(test.exists(9));
            }

            THEN("The element should be replaced with its right child")
            {
                REQUIRE(test.get_root()->right->right->right->value == 10);
                CHECK(test.get_root()->right->right->right->is_black());
            }

            THEN("The size of the allocator should decrease")
            {
                REQUIRE(test.get_allocator().size() == 10);
            }
        }

        WHEN("An element that exists with only one child(left child) is deleted")
        {
            test.insert(0);

            test.erase(1);

            THEN("The element shouldn't exist")
            {
                CHECK_FALSE(test.exists(1));
            }

            THEN("The element should be replaced with its right child")
            {
                REQUIRE(test.get_root()->left->left->value == 0);
                CHECK(test.get_root()->left->left->is_black());
            }

            THEN("The size of the allocator should decrease")
            {
                REQUIRE(test.get_allocator().size() == 11);
            }
        }

        WHEN("An existing element with 2 children is deleted")
        {
            THEN("If the parent of the successor is the deleted node")
            {
                test.erase(8);

                THEN("The element shouldn't exist")
                {
                    CHECK_FALSE(test.exists(8));
                }

                THEN("The element should be replaced with its right child")
                {
                    REQUIRE(test.get_root()->right->right->right->value == 10);
                    CHECK(test.get_root()->right->right->right->is_black());
                }

                THEN("The size of the allocator should decrease")
                {
                    REQUIRE(test.get_allocator().size() == 10);
                }
            }

            THEN("If the parent of the successor isn't the deleted node")
            {
                test.erase(6);

                THEN("The element shouldn't exist")
                {
                    CHECK_FALSE(test.exists(6));
                }

                THEN("The root shouldn't change")
                {
                    REQUIRE(test.get_root()->value == 4);
                    CHECK(test.get_root()->is_black());
                }

                THEN("The left subtree shoudn't change")
                {
                    REQUIRE(test.get_root()->left->value == 2);
                    CHECK(test.get_root()->left->is_black());

                    REQUIRE(test.get_root()->left->right->value == 3);
                    CHECK(test.get_root()->left->right->is_black());

                    REQUIRE(test.get_root()->left->left->value == 1);
                    CHECK(test.get_root()->left->left->is_black());
                }

                THEN("The right subtree should be valid")
                {
                    REQUIRE(test.get_root()->right->value == 7);
                    CHECK(test.get_root()->right->is_black());

                    REQUIRE(test.get_root()->right->left->value == 5);
                    CHECK(test.get_root()->right->left->is_black());

                    REQUIRE(test.get_root()->right->right->value == 9);
                    CHECK(test.get_root()->right->right->is_red());

                    REQUIRE(test.get_root()->right->value == 7);
                    CHECK(test.get_root()->right->is_black());

                    REQUIRE(test.get_root()->right->right->left->value == 8);
                    CHECK(test.get_root()->right->right->left->is_black());
                }

                THEN("The size of the allocator should decrease")
                {
                    REQUIRE(test.get_allocator().size() == 10);
                }
            }
        }
    
    }
}

SCENARIO("Testing exists function")
{
    GIVEN("A non-empty tree")
    {
        tree test;

        init_tree(test);

        WHEN("An existing element is searched")
        {
            THEN("Exists function should return true")
            {
                CHECK(test.exists(2));
                CHECK(test.exists(4));
                CHECK(test.exists(10));
            }
        }

        WHEN("An element that doesn't exist is searched")
        {
            THEN("Exists should return false")
            {
                CHECK_FALSE(test.exists(0));
                CHECK_FALSE(test.exists(15));
            }
        }
    }

    GIVEN("A non-empty constant tree")
    {
        tree test;
        init_tree(test);

        const tree ctest(test);

        WHEN("An existing element is searched")
        {
            THEN("Exists function should return true")
            {
                CHECK(ctest.exists(2));
                CHECK(ctest.exists(4));
                CHECK(ctest.exists(10));
            }
        }

        WHEN("An element that doesn't exist is searched")
        {
            THEN("Exists should return false")
            {
                CHECK_FALSE(ctest.exists(0));
                CHECK_FALSE(ctest.exists(15));
            }
        }
    }
}

SCENARIO("Testing black height function")
{
    GIVEN("An empty tree")
    {
        tree test;

        THEN("Black height should be 0")
        {
            REQUIRE(test.black_height() == 0);
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;

        init_tree(test);

        THEN("Black height should be 3")
        {
            REQUIRE(test.black_height() == 3);
        }

        WHEN("More elements are inserted so that blakc height doesn't change")
        {
            for(size_t i = 11; i <= 18; ++i)
                test.insert(i);

            THEN("Black height should be correct")
            {
                REQUIRE(test.black_height() == 3);
            }
        }
    }

    GIVEN("A non-empty constant tree")
    {
        tree test;
        init_tree(test);

        const tree ctest(test);

        THEN("Black height should be 3")
        {
            REQUIRE(ctest.black_height() == 3);
        }
    }
}

SCENARIO("Testing get_allocator function")
{
    GIVEN("An empty tree")
    {
        tree test;

        THEN("The allocator returned by the function should be valid")
        {
            REQUIRE(test.get_allocator().size() == 1);
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;

        init_tree(test);

        THEN("The allocator returned by the function should be valid")
        {
            REQUIRE(test.get_allocator().size() == 11);
        }
    }
}

SCENARIO("Testing height function")
{
    GIVEN("An empty tree")
    {
        tree test;

        THEN("Height should be 0")
        {
            REQUIRE(test.height() == 0); 
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        THEN("Calculate_height should return the height of the tree")
        {
            REQUIRE(test.height() == 5);
        }
    }

    GIVEN("A non-empty constant tree")
    {
        tree test;
        init_tree(test);

        const tree ctree(test);

        THEN("Calculate_height should return the height of the tree")
        {
            REQUIRE(ctree.height() == 5);
        }
    }
}

SCENARIO("Testing empty function")
{
    GIVEN("An empty tree")
    {
        tree test;

        THEN("Empty should return true")
        {
            CHECK(test.empty());
        }
    }

    GIVEN("An empty constant tree")
    {
        const tree test;

        THEN("Empty should return true")
        {
            CHECK(test.empty());
        }
    }

    
    GIVEN("A non-empty tree")
    {
        tree test;

        init_tree(test);

        THEN("Empty should return false")
        {
            CHECK_FALSE(test.empty());
        }
    }
    
    GIVEN("A non-empty constant tree")
    {
        tree test;
        init_tree(test);

        const tree ctest(test);

        THEN("Empty should return false")
        {
            CHECK_FALSE(ctest.empty());
        }
    }
}

SCENARIO("Testing clear function")
{
    GIVEN("An empty tree")
    {
        tree test;

        WHEN("The tree is cleared")
        {
            test.clear();

            THEN("The tree should remain empty")
            {
                CHECK(test.empty());
            }

            THEN("The allocator size shouldn't change")
            {
                REQUIRE(test.get_allocator().size() == 1);
            }
        
        }
    }

    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        WHEN("The tree is cleared")
        {
            test.clear();

            THEN("The tree should be empty")
            {
                CHECK(test.empty());
            }

            THEN("The size of the allocator should be 1")
            {
                REQUIRE(test.get_allocator().size() == 1);
            }
        }
    }
}