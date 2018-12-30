#include "catch.hpp"

#include "../include/set_share_element.hpp"

#include <set>

TEST_CASE("set_share_element")
{
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 3, 5}));
    REQUIRE(!set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 5, 7}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{2, 3}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{4, 5}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 2}));
    REQUIRE(!set_share_element(std::set<int>{2, 3, 4}, std::set<int>{}));
    REQUIRE(!set_share_element(std::set<int>{}, std::set<int>{2, 3, 4}));
    REQUIRE(!set_share_element(std::set<int>{}, std::set<int>{}));
}
