#include "catch.hpp"

#include "algorithm.hpp"
#include "set_logical_operation.hpp"

#include <set>
#include <vector>

TEST_CASE("set_logical_operation")
{
    std::set<int> left = {1, 2, 3, 5, 7, 9};
    std::set<int> right = {2, 4, 5, 6, 10};
    
    std::vector<int> expected;
    std::vector<int> results;
    
    SECTION("none")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return false;});
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("and")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft && inRight;});
        std::set_intersection(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("left only")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft && !inRight;});
        std::set_difference(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("left only")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft;});
        expected.assign(begin(left), end(left));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("right only")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return !inLeft && inRight;});
        std::set_difference(begin(right), end(right), begin(left), end(left), std::back_inserter(expected));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("right")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inRight;});
        expected.assign(begin(right), end(right));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("right")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft ^ inRight;});
        std::set_symmetric_difference(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
        REQUIRE(ranges::equal(results, expected));
    }
    
    SECTION("or")
    {
        fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft || inRight;});
        std::set_union(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
        REQUIRE(ranges::equal(results, expected));
    }
}

