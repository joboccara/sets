#include "catch.hpp"

#include "../include/set_bool_algorithms.hpp"

#include <set>

TEST_CASE("set_share_element")
{
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 3, 5}));
    REQUIRE(set_share_element(std::set<int>{1, 3, 5}, std::set<int>{2, 3, 4}));
    REQUIRE(!set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 5, 7}));
    REQUIRE(!set_share_element(std::set<int>{1, 5, 7}, std::set<int>{2, 3, 4}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{2, 3}));
    REQUIRE(set_share_element(std::set<int>{2, 3}, std::set<int>{2, 3, 4}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{4, 5}));
    REQUIRE(set_share_element(std::set<int>{4, 5}, std::set<int>{2, 3, 4}));
    REQUIRE(set_share_element(std::set<int>{2, 3, 4}, std::set<int>{1, 2}));
    REQUIRE(set_share_element(std::set<int>{1, 2}, std::set<int>{2, 3, 4}));
    REQUIRE(!set_share_element(std::set<int>{2, 3, 4}, std::set<int>{}));
    REQUIRE(!set_share_element(std::set<int>{}, std::set<int>{2, 3, 4}));
    REQUIRE(!set_share_element(std::set<int>{}, std::set<int>{}));
}

template <typename Range1, typename Range2>
bool isSameAsStdIncludes(Range1 const& range1, Range2 const& range2)
{
    return includes(range1, range2) == std::includes(begin(range1), end(range1), begin(range2), end(range2));
}

TEST_CASE("includes")
{
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{2,3}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{2,3,4}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{2,4,5}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{5,6}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{0,1,2}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{0,1}));
    REQUIRE(isSameAsStdIncludes(std::set<int>{1,2,3,4}, std::set<int>{0,6,7}));
}

TEST_CASE("is_prefix_of")
{
    using set = std::set<int>;
    
    REQUIRE      (is_prefix_of(set{1, 2, 3, 4}, set{1, 2, 3, 4, 5, 6}));
    
    REQUIRE      (is_prefix_of(set{},           set{1, 2, 3, 4}));
    REQUIRE_FALSE(is_prefix_of(set{1, 2, 3, 4}, set{}));
    REQUIRE      (is_prefix_of(set{},           set{1, 2, 3, 4}));
    REQUIRE      (is_prefix_of(set{},           set{}));

    REQUIRE_FALSE(is_prefix_of(set{1, 2, 3, 4, 5, 6}, set{1, 2, 3, 4}));
    REQUIRE_FALSE(is_prefix_of(set{2, 3, 4, 5}, set{1, 2, 3, 4, 5}));
    REQUIRE_FALSE(is_prefix_of(set{2, 3, 4},    set{1, 2, 3, 4, 5}));
    REQUIRE_FALSE(is_prefix_of(set{2, 3, 4},    set{1, 2, 3, 5}));
    REQUIRE_FALSE(is_prefix_of(set{1, 3, 4},    set{1, 2, 3, 4}));
}

TEST_CASE("is_prefix_of_other")
{
    using set = std::set<int>;
    
    REQUIRE      (is_prefix_of_other(set{1, 2, 3, 4}, set{1, 2, 3, 4, 5, 6}));
    REQUIRE      (is_prefix_of_other(set{1, 2, 3, 4, 5, 6}, set{1, 2, 3, 4}));

    REQUIRE      (is_prefix_of_other(set{},           set{1, 2, 3, 4}));
    REQUIRE      (is_prefix_of_other(set{1, 2, 3, 4}, set{}));
    REQUIRE      (is_prefix_of_other(set{},           set{1, 2, 3, 4}));
    REQUIRE      (is_prefix_of_other(set{},           set{}));

    REQUIRE_FALSE(is_prefix_of_other(set{2, 3, 4, 5}, set{1, 2, 3, 4, 5}));
    REQUIRE_FALSE(is_prefix_of_other(set{2, 3, 4},    set{1, 2, 3, 4, 5}));
    REQUIRE_FALSE(is_prefix_of_other(set{2, 3, 4},    set{1, 2, 3, 5}));
    REQUIRE_FALSE(is_prefix_of_other(set{1, 3, 4},    set{1, 2, 3, 4}));
}
