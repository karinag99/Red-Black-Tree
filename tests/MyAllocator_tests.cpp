#include "catch.hpp"
#include "MyAllocator.hpp"

SCENARIO("Testing allocator behavior")
{
    GIVEN("An empty allocator")
    {
       MyAllocator<int> alloc;
   
        THEN("The size of allocated elements should be zero")
        {
            REQUIRE(alloc.size() == 0);
        }

        WHEN("A new element is allocated with default constructor")
        {
            int* new_elem = alloc.allocate();

            THEN("The size should increase")
            {
                REQUIRE(alloc.size() == 1);
            }

            THEN("The new element should be allocated correctly")
            {
                CHECK(alloc.is_allocated(new_elem));
            }
        }

        WHEN("A new element is allocated with parameterized constructor")
        {
            int* new_elem = alloc.allocate(5);

            THEN("The size should increase")
            {
                REQUIRE(alloc.size() == 1);
            }

            THEN("The new element should be allocated correctly")
            {
                CHECK(alloc.is_allocated(new_elem));
            }

            THEN("The new element should be with valid")
            {
                REQUIRE(*new_elem == 5);
            }
        }   
    }

    GIVEN("Given a non-empty allocator")
    {
        MyAllocator<int> alloc;

        int* fst_elem = alloc.allocate();
        int* snd_elem = alloc.allocate(3);
        int* trd_elem = alloc.allocate(7);

        THEN("Size should be valid")
        {
            REQUIRE(alloc.size() == 3);
        }

        THEN("The elements should be allocated correctly")
        {
            CHECK(alloc.is_allocated(fst_elem));
            CHECK(alloc.is_allocated(snd_elem));
            CHECK(alloc.is_allocated(trd_elem));
        }

        THEN("The elements should be valid ")
        {
            REQUIRE(*snd_elem == 3);
            REQUIRE(*trd_elem == 7);
        }

        WHEN("A new element is allocated")
        {
            int* frt_elem = alloc.allocate(8);

            THEN("Size should increase")
            {
                REQUIRE(alloc.size() == 4);
            }

            THEN("The new element should be allocated correctly")
            {
                CHECK(alloc.is_allocated(frt_elem));
            }

            THEN("The new element should be valid")
            {
                REQUIRE(*frt_elem == 8);
            }
        }

        WHEN("An element that is not beeing allocated is deallocated")
        {
            int* not_allocated = new int(10);

            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS_AS(alloc.deallocate(not_allocated), std::invalid_argument);
            }
        }

        WHEN("An allocated element is deallocated")
        {
            alloc.deallocate(snd_elem);

            THEN("The element should be inaccessible")
            {
                CHECK_FALSE(alloc.is_allocated(snd_elem));
            }

            THEN("The size should decrease")
            {
                REQUIRE(alloc.size() == 2);
            }

            THEN("Other elements should stay allocated")
            {
                CHECK(alloc.is_allocated(fst_elem));
                CHECK(alloc.is_allocated(trd_elem));
            }

            THEN("Other elements should be valid")
            {
                REQUIRE(*trd_elem == 7);
            }
        }
    }
}