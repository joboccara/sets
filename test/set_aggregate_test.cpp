#include "catch.hpp"

#include "../include/detail/algorithm.hpp"
#include "../include/set_aggregate.hpp"

#include <map>
#include <string>
#include <utility>

TEST_CASE("set_aggregate")
{
    std::map<int, std::string> left = {{1, "a"}, {2, "b"}, {3, "c1"}, {5, "e1"}, {7, "g1"}, {9, "i"}};
    std::map<int, std::string> right = {{3, "c2"}, {4, "d"}, {5, "e2"}, {6, "f"},  {7, "g2"}};
    
    std::vector<std::pair<int, std::string>> expectedPermutation = {{1, "a"}, {2, "b"}, {3, "c1c2"}, {4, "d"}, {5, "e1e2"}, {6, "f"}, {7, "g1g2"}, {9, "i"}};
    std::vector<std::pair<int, std::string>> results;
    fluent::set_aggregate(left, right, std::back_inserter(results),
                          [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2){ return p1.first < p2.first; },
                          [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2){ return std::make_pair(p1.first, p1.second + p2.second); });
    
    REQUIRE(ranges::is_permutation(results, expectedPermutation));
}
