#include "catch.hpp"
#include "RBTreeTest.hpp"

SCENARIO("Testing rotations")
{
    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        tree copy_test(test);

        WHEN("A left rotation from the root is performed")
        {
            test.rotateLeft(test.get_root());

            THEN("The new root should be the right child of the old root")
            {
                CHECK(are_equal_nodes(test.get_root(), copy_test.get_root()->right));
            }

            THEN("The right child of the new root shouldn't change")
            {
                CHECK(are_equal_nodes(test.get_root()->right, copy_test.get_root()->right->right));
                CHECK(have_same_parents(test.get_root()->right, copy_test.get_root()->right->right));
            }

            THEN("The left child of the left child of the new root shouldn't change")
            {
                CHECK(are_equal_nodes(test.get_root()->left->left, copy_test.get_root()->left));
                CHECK(have_same_parents(test.get_root()->left->left, copy_test.get_root()->left));
            }

            THEN("The right child of the left child of the new root should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->left->right, copy_test.get_root()->right->left));
                //REQUIRE(test.get_root()->left->right->parent->value == copy_test.get_root()->right->value);
            }

            THEN("The right child of the new root should be valid")
            {
                REQUIRE(test.get_root()->right->value == copy_test.get_root()->right->right->value);
                REQUIRE(test.get_root()->right->parent->value == copy_test.get_root()->right->value);
            }
        }

        WHEN("A left rotation on an internal node - right child is performed")
        {
            test.rotateLeft(test.get_root()->right);

            THEN("The parent of the new get_get_root()() of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->right->parent, copy_test.get_root()->right->parent));
            }

            THEN("The parent's right child of the old root of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->right, copy_test.get_root()->right->right));
            }
        }

        WHEN("A left rotation on an internal node - left child is performed")
        {
            test.rotateLeft(test.get_root()->left);

            THEN("The parent of the new root of the subtree shpuld be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->left->parent, copy_test.get_root()));
            }

            THEN("The parent's left child of the old root of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->left, copy_test.get_root()->left->right));
            }
        }

         WHEN("A right rotation from the root is performed")
        {
            test.rotateRight(test.get_root());

            THEN("The new root should be the left child of the old root")
            {
                CHECK(are_equal_nodes(test.get_root(), copy_test.get_root()->left));
            }

            THEN("The left child of the new root shouldn't change")
            {
                CHECK(are_equal_nodes(test.get_root()->left, copy_test.get_root()->left->left));
                CHECK(have_same_parents(test.get_root()->left, copy_test.get_root()->left->left));
            }

            THEN("The right child of the right child of the new root shouldn't change")
            {
                CHECK(are_equal_nodes(test.get_root()->right->right, copy_test.get_root()->right));
                CHECK(have_same_parents(test.get_root()->right->right, copy_test.get_root()->right));
            }

            THEN("The left child of the right child of the new root should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->right->left, copy_test.get_root()->left->right));
                REQUIRE(test.get_root()->right->left->parent->value == copy_test.get_root()->value);
            }

            THEN("The left child of the new root should be valid")
            {
                REQUIRE(test.get_root()->left->value == copy_test.get_root()->left->left->value);
                REQUIRE(test.get_root()->left->parent->value == copy_test.get_root()->left->value);
            }
        }

         WHEN("A right rotation on an internal node - left child is performed")
        {
            test.rotateRight(test.get_root()->left);

            THEN("The parent of the new root of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->left->parent, copy_test.get_root()->left->parent));
            }

            THEN("The parent's left child of the old root of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->left, copy_test.get_root()->left->left));
            }
        }

        WHEN("A right rotation on an internal node - right child is performed")
        {
            test.rotateRight(test.get_root()->right);

            THEN("The parent of the new root of the subtree shpuld be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->right->parent, copy_test.get_root()->right->parent));
            }

            THEN("The parent's right child of the old root of the subtree should be valid")
            {
                CHECK(are_equal_nodes(test.get_root()->right, copy_test.get_root()->right->left));
            }
        }
    }
}

SCENARIO("Testing transplant function")
{
    GIVEN("A non-empty tree")
    {
        tree test;
        init_tree(test);

        WHEN("The tree from the root is replaced with a subtree")
        {
            node_ptr old_root = test.get_root();
            node_ptr new_root = test.get_root()->right->left;

            test.transplant(old_root, new_root);

            old_root->right->left = test.get_null_node();

            THEN("The new root should be the root of the subtree")
            {
                REQUIRE(test.get_root() == new_root);
            }

            THEN("The parent of the root of the subtree should be the null node")
            {
                REQUIRE(test.get_root()->parent == test.get_null_node());
            }

            test.delete_not_null_nodes(old_root);
        }

        WHEN("A subtree with a root(left child) is replaced")
        {
            node_ptr replaced_tree = test.get_root()->left;
            node_ptr subtree = test.get_root()->left->right;

            test.transplant(replaced_tree, test.get_root()->left->right);

            replaced_tree->right = test.get_null_node();
        
            THEN("The left child of the parent of the replaced tree should be valid")
            {
                REQUIRE(test.get_root()->left == subtree);
            }

            THEN("The parent of the subtree should be valid")
            {
                REQUIRE(subtree->parent == test.get_root());
            }

            test.delete_not_null_nodes(replaced_tree);
        }

        WHEN("A subtree with a root(right child) is replaced")
        {
            node_ptr replaced_tree = test.get_root()->right;
            node_ptr subtree = test.get_root()->right->right;

            test.transplant(replaced_tree, test.get_root()->right->right);

            replaced_tree->right = test.get_null_node();
        
            THEN("The rigth child of the parent of the replaced tree should be valid")
            {
                REQUIRE(test.get_root()->right == subtree);
            }

            THEN("The parent of the subtree should be valid")
            {
                REQUIRE(subtree->parent == test.get_root());
            }

            test.delete_not_null_nodes(replaced_tree);
        }
    }
}

SCENARIO("Testing insert_fixup function")
{
    //check if we reach the llop more than once
    //check if we dont reach the loop
    GIVEN("A non-empty tree")
    {
        tree test;

        test.insert(12);
        test.insert(15);
        test.insert(10);

        WHEN("A new element is added so that it's parent is a left child")
        {
            THEN("If we fall in case 1")
            {
                THEN("If the new node is a left child")
                {
                    test.insert(8);

                    THEN("The new node should be red and with valid value")
                    {
                        CHECK(test.get_root()->left->left->is_red());
                        REQUIRE(test.get_root()->left->left->value == 8);
                    }

                    THEN("The parent of the new node should be black and with a valid value")
                    {
                        CHECK(test.get_root()->left->is_black());
                        REQUIRE(test.get_root()->left->value == 10);
                    }

                    THEN("The sibling of the parent should be black")
                    {
                        CHECK(test.get_root()->right->is_black());
                        REQUIRE(test.get_root()->right->value == 15);
                    }

                    THEN("The root should be black")
                    {
                        CHECK(test.get_root()->is_black());
                        REQUIRE(test.get_root()->value == 12);
                    }
                }

                THEN("If the new node is a right child")
                {
                    test.insert(11);

                    THEN("The new node should be red and with valid value")
                    {
                        CHECK(test.get_root()->left->right->is_red());
                        REQUIRE(test.get_root()->left->right->value == 11);
                    }

                    THEN("The parent of the new node should be black and with valid value")
                    {
                        CHECK(test.get_root()->left->is_black());
                        REQUIRE(test.get_root()->left->value == 10);
                    }

                    THEN("The sibling of the parent should be black and with valid value")
                    {
                        CHECK(test.get_root()->right->is_black());
                        REQUIRE(test.get_root()->right->value == 15);
                    }

                    THEN("The root should be black and with valid value")
                    {
                        CHECK(test.get_root()->is_black());
                        REQUIRE(test.get_root()->value == 12);
                    }
                }
            }

            test.insert(7); //case one

            THEN("If we fall in case 3")
            {
                test.insert(6);

                THEN("The root should be black and with valid value")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 12);
                }

                THEN("The right child of the root should remain black and with valid value")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 15);
                }

                THEN("The left child of the root should be the parent of the new node - 8")
                {
                    REQUIRE(test.get_root()->left->value == 7);
                }

                THEN("The parent of the new node should be black")
                {
                    CHECK(test.get_root()->left->is_black());
                }

                THEN("The new node should be red and with valid value")
                {
                    CHECK(test.get_root()->left->left->is_red());
                    REQUIRE(test.get_root()->left->left->value == 6);
                }

                THEN("The left child of the root before the insertion should be sibling of the new node")
                {
                    REQUIRE(test.get_root()->left->right->value == 10);
                }

                THEN("The sibling of the new node should be red")
                {
                    CHECK(test.get_root()->left->right->is_red());
                }
            }

            THEN("If we fall in case 2")
            {
                test.insert(9);

                THEN("The root should't change")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 12);
                }

                THEN("The right child of the root should't change")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 15);
                }

                THEN("The left child of the root should be black")
                {
                    CHECK(test.get_root()->left->is_black());
                }

                THEN("The left child of the root should contain the inserted value")
                {
                    REQUIRE(test.get_root()->left->value == 9);
                }

                THEN("The left and the right child of the new node should be red")
                {
                    CHECK(test.get_root()->left->left->is_red());
                    CHECK(test.get_root()->left->right->is_red());
                }

                THEN("The left child of the new node should contain valid value")
                {
                    REQUIRE(test.get_root()->left->left->value == 7);
                }

                THEN("The right child of the new node should contain valid value")
                {
                    REQUIRE(test.get_root()->left->right->value == 10);
                }
            }
        }

        WHEN("A new element is added so that it's parent is a right child")
        {
            THEN("IF we fall in case one")
            {
                THEN("If the new node is a right child")
                {
                    test.insert(16);

                    THEN("The new node should be red and with valid value")
                    {
                        CHECK(test.get_root()->right->right->is_red());
                        REQUIRE(test.get_root()->right->right->value == 16);
                    }

                    THEN("The parent of the new node should be black and with a valid value")
                    {
                        CHECK(test.get_root()->right->is_black());
                        REQUIRE(test.get_root()->right->value == 15);
                    }

                    THEN("The sibling of the parent should be black")
                    {
                        CHECK(test.get_root()->left->is_black());
                        REQUIRE(test.get_root()->left->value == 10);
                    }

                    THEN("The root should be black")
                    {
                        CHECK(test.get_root()->is_black());
                        REQUIRE(test.get_root()->value == 12);
                    }
                }

                THEN("If the new node is a left child")
                {
                    test.insert(13);

                    THEN("The new node should be red and with valid value")
                    {
                        CHECK(test.get_root()->right->left->is_red());
                        REQUIRE(test.get_root()->right->left->value == 13);
                    }

                    THEN("The parent of the new node should be black and with valid value")
                    {
                        CHECK(test.get_root()->right->is_black());
                        REQUIRE(test.get_root()->right->value == 15);
                    }

                    THEN("The sibling of the parent should be black and with valid value")
                    {
                        CHECK(test.get_root()->left->is_black());
                        REQUIRE(test.get_root()->left->value == 10);
                    }

                    THEN("The root should be black and with valid value")
                    {
                        CHECK(test.get_root()->is_black());
                        REQUIRE(test.get_root()->value == 12);
                    }
                }
            }

            test.insert(18);

            THEN("If we fall in case 3")
            {
                test.insert(20);

                THEN("The root should be black and with valid value")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 12);
                }

                THEN("The left child of the root should remain black and with valid value")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 10);
                }

                THEN("The right child of the root should be the parent of the new node - 18")
                {
                    REQUIRE(test.get_root()->right->value == 18);
                }

                THEN("The parent of the new node should be black")
                {
                    CHECK(test.get_root()->left->is_black());
                }

                THEN("The new node should be red and with valid value")
                {
                    CHECK(test.get_root()->right->right->is_red());
                    REQUIRE(test.get_root()->right->right->value == 20);
                }

                THEN("The right child of the root before the insertion should be sibling of the new node")
                {
                    REQUIRE(test.get_root()->right->left->value == 15);
                }

                THEN("The sibling of the new node should be red")
                {
                    CHECK(test.get_root()->right->left->is_red());
                }
            }
        
            THEN("If we fall in case 2")
            {
                test.insert(16);

                THEN("The root should't change")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 12);
                }

                THEN("The left child of the root should't change")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 10);
                }

                THEN("The right child of the root should be black")
                {
                    CHECK(test.get_root()->right->is_black());
                }

                THEN("The right child of the root should contain the inserted value")
                {
                    REQUIRE(test.get_root()->right->value == 16);
                }

                THEN("The right and the left child of the new node should be red")
                {
                    CHECK(test.get_root()->right->right->is_red());
                    CHECK(test.get_root()->right->left->is_red());
                }

                THEN("The right child of the new node should contain valid value")
                {
                    REQUIRE(test.get_root()->right->right->value == 18);
                }

                THEN("The left child of the new node should contain valid value")
                {
                    REQUIRE(test.get_root()->right->left->value == 15);
                }
            }
        }
    }
}

SCENARIO("Testing delete_fixup function")
{
    GIVEN("A non-empty tree")
    {
        tree test;
        test.insert(30);
        test.insert(10);
        test.insert(40);

        WHEN("An element is deleted and the fixup node is a left child")
        {
            test.insert(35);
            test.insert(45);
            //check for nullptr - deleted node
            THEN("If we fall in case 4")
            {
                test.erase(10);

                THEN("The new root should be black and should contain the value of the old root's right child")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 40);
                }

                THEN("The right child of the new root should be a black leaf with the same value")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 45);

                    CHECK(is_leaf(test.get_root()->right, test.get_null_node()));
                }

                THEN("The left child of the new root should be black and should contain the value of the old root")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 30);
                }

                THEN("The right child of the root's left child should be a red leaf and with valid value")
                {
                    CHECK(test.get_root()->left->right->is_red());
                    REQUIRE(test.get_root()->left->right->value == 35);

                    CHECK(is_leaf(test.get_root()->left->right, test.get_null_node()));
                }

                THEN("The left child of the old root(right child of the new root) should be null node")
                {
                    REQUIRE(test.get_root()->left->left == test.get_null_node());
                }
            }

            THEN("If we fall in case 1 and case 2")
            {
                test.insert(50);

                test.erase(10);

                THEN("The root should be black and should contain the value of the right child of the old root")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 40);
                }

                THEN("The left child of the new root should be black and should contain the value of the old root")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 30);
                }

                THEN("The right child of the new root should be black and should contain valid value")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 45);
                }

                THEN("The right child of the new root's right child should be a red leaf and should contain valid value")
                {
                    CHECK(test.get_root()->right->right->is_red());
                    REQUIRE(test.get_root()->right->right->value == 50);

                    CHECK(is_leaf(test.get_root()->right->right, test.get_null_node()));
                }

                THEN("The right child of the root's left child should be a red leaf with valid value")
                {
                    CHECK(test.get_root()->left->right->is_red());
                    REQUIRE(test.get_root()->left->right->value == 35);

                    CHECK(is_leaf(test.get_root()->left->right ,test.get_null_node()));
                }

                THEN("The left child of the root's left child should be the null node")
                {
                    REQUIRE(test.get_root()->left->left == test.get_null_node());
                }
            }

            THEN("If we fall in case 3 (and then case 4)")
            {
                test.insert(50);
                
                test.insert(34);
                test.insert(38);
                test.insert(32);

                test.erase(10);

                THEN("The root shouldn't change")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 35);
                }

                THEN("The right subtree of the root shouldn't change")
                {
                    CHECK(test.get_root()->right->is_red());
                    REQUIRE(test.get_root()->right->value == 40);

                    CHECK(test.get_root()->right->left->is_black());
                    REQUIRE(test.get_root()->right->left->value == 38);

                    CHECK(test.get_root()->right->right->is_black());
                    REQUIRE(test.get_root()->right->right->value == 45);

                    CHECK(test.get_root()->right->right->right->is_red());
                    REQUIRE(test.get_root()->right->right->right->value == 50);

                    REQUIRE(test.get_root()->right->right->left == test.get_null_node());

                    CHECK(is_leaf(test.get_root()->right->left, test.get_null_node()));
                    CHECK(is_leaf(test.get_root()->right->right->right, test.get_null_node()));
                }

                THEN("The left child of the root should be red")
                {
                    CHECK(test.get_root()->left->is_red());
                }

                THEN("The left child of the root should contain the value of the right child of the sibling if the deleted node")
                {
                    REQUIRE(test.get_root()->left->value == 32);
                }

                THEN("The children of the left child of the root should be black and with no children")
                {
                    CHECK(test.get_root()->left->left->is_black());
                    CHECK(test.get_root()->left->right->is_black());

                    CHECK(is_leaf(test.get_root()->left->left, test.get_null_node()));
                    CHECK(is_leaf(test.get_root()->left->right, test.get_null_node()));
                }

                THEN("The left child of the left child of the root should contain the value of the parent of the deleted node")
                {
                    REQUIRE(test.get_root()->left->left->value == 30);
                }

                THEN("The right child of the left child of the root should contain the value of the sibling of the deleted node")
                {
                    REQUIRE(test.get_root()->left->right->value == 34);
                }               
            }
        }

        WHEN("An element is deleted and the fixup node is a right child")
        {
            test.insert(5);
            test.insert(20);

            THEN("If we fall in case 4")
            {
                test.erase(40);

                THEN("The new root should be black and should contain the value of the old root's left child")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 10);
                }

                THEN("The right child of the new root should be black and should contain the vlaue of the old root")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 30);
                }

                THEN("The left child of the new root should be a black leaf with the same value")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 5);

                    CHECK(is_leaf(test.get_root()->left, test.get_null_node()));
                }

                THEN("The left child of the root's right child should be a red leaf and with valid value")
                {
                    CHECK(test.get_root()->right->left->is_red());
                    REQUIRE(test.get_root()->right->left->value == 20);

                    CHECK(is_leaf(test.get_root()->right->left ,test.get_null_node()));
                }

                THEN("The right child of the old root(left child of the new root) should be null node")
                {
                    REQUIRE(test.get_root()->right->right == test.get_null_node());
                }
            }
        
            THEN("If we fall in case 1 and case 2")
            {
                test.insert(2);

                test.erase(40);

                THEN("The root should be black and should contain the value of the left child of the old root")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 10);
                }

                THEN("The right child of the new root should be black and should contain the value of the old root")
                {
                    CHECK(test.get_root()->right->is_black());
                    REQUIRE(test.get_root()->right->value == 30);
                }

                THEN("The left child of the new root should be black and should contain valid value")
                {
                    CHECK(test.get_root()->left->is_black());
                    REQUIRE(test.get_root()->left->value == 5);
                }

                THEN("The left child of the new root's left child should be a red leaf and should contain valid value")
                {
                    CHECK(test.get_root()->left->left->is_red());
                    REQUIRE(test.get_root()->left->left->value == 2);

                    CHECK(is_leaf(test.get_root()->left->left, test.get_null_node()));
                }

                THEN("The left child of the root's right child should be a red leaf with valid value")
                {
                    CHECK(test.get_root()->right->left->is_red());
                    REQUIRE(test.get_root()->right->left->value == 20);

                    CHECK(is_leaf(test.get_root()->right->left, test.get_null_node()));
                }

                THEN("The right child of the root's right child should be the null node")
                {
                    REQUIRE(test.get_root()->right->right == test.get_null_node());
                }
            }

            THEN("If we fall in case 3 (and then case 4)")
            {
                test.insert(2);
                
                test.insert(35);
                test.insert(38);
                test.insert(36);

                test.erase(40);

                THEN("The root shouldn't change")
                {
                    CHECK(test.get_root()->is_black());
                    REQUIRE(test.get_root()->value == 30);
                }

                THEN("The left subtree of the root shouldn't change")
                {
                    CHECK(test.get_root()->left->is_red());
                    REQUIRE(test.get_root()->left->value == 10);

                    CHECK(test.get_root()->left->right->is_black());
                    REQUIRE(test.get_root()->left->right->value == 20);

                    CHECK(test.get_root()->left->left->is_black());
                    REQUIRE(test.get_root()->left->left->value == 5);

                    CHECK(test.get_root()->left->left->left->is_red());
                    REQUIRE(test.get_root()->left->left->left->value == 2);

                    REQUIRE(test.get_root()->left->left->right == test.get_null_node());

                    CHECK(is_leaf(test.get_root()->left->right, test.get_null_node()));
                    CHECK(is_leaf(test.get_root()->left->left->left, test.get_null_node()));
                }

                THEN("The right child of the root should be red")
                {
                    CHECK(test.get_root()->right->is_red());
                }

                THEN("The right child of the root should contain the value of the right child of the sibling of the deleted node")
                {
                    REQUIRE(test.get_root()->right->value == 36);
                }

                THEN("The children of the right child of the root should be black and with no children")
                {
                    CHECK(test.get_root()->right->right->is_black());
                    CHECK(test.get_root()->right->left->is_black());

                    CHECK(is_leaf(test.get_root()->right->right, test.get_null_node()));
                    CHECK(is_leaf(test.get_root()->right->left, test.get_null_node()));
                }
            
                THEN("The right child of the right child of the root should contain the value of the parent of the deleted node")
                {
                    REQUIRE(test.get_root()->right->right->value == 38);
                }
            
                THEN("The left child of the right child of the root should contain the value of the sibling of the deleted node")
                {
                    REQUIRE(test.get_root()->right->left->value == 35);
                }
            }
        }
    }
}




