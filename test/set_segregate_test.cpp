#include "catch.hpp"
#include "../include/detail/algorithm.hpp"
#include "../include/set_segregate.hpp"

#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

template<typename T>
std::vector<T> tee(std::vector<T> const& values)
{
    std::copy(values.begin(), values.end(), std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
    return values;
}

struct SequenceOutputInsertion
{
    template<typename SequenceContainer>
    auto operator()(SequenceContainer& container) -> decltype(std::back_inserter(container))
    {
        return std::back_inserter(container);
    }
};

struct AssociativeOutputInsertion
{
    template<typename AssociativeContainer>
    auto operator()(AssociativeContainer& container) -> decltype(std::inserter(container, container.end()))
    {
        return std::inserter(container, container.end());
    }
};

template<typename LeftOnlyOutputInsertion = SequenceOutputInsertion,
typename BothOutputInsertion = LeftOnlyOutputInsertion,
typename RightOnlyOutputInsertion = LeftOnlyOutputInsertion,
typename RangeLeft, typename RangeRight,
typename RangeLeftOnly, typename RangeBoth, typename RangeRightOnly>
bool setSegregateOutputsExpected(RangeLeft const& left, RangeRight const& right,
                      RangeLeftOnly const& expectedLeftOnly, RangeBoth const& expectedBoth, RangeRightOnly const& expectedRightOnly)
{
    RangeLeftOnly leftOnly;
    RangeBoth both;
    RangeRightOnly rightOnly;
    
    fluent::set_segregate(left, right, LeftOnlyOutputInsertion()(leftOnly), BothOutputInsertion()(both), RightOnlyOutputInsertion()(rightOnly));
    
    return ranges::equal(leftOnly, expectedLeftOnly)
    && ranges::equal(both, expectedBoth)
    && ranges::equal(rightOnly, expectedRightOnly);
}

template<typename LeftOnlyOutputInsertion = SequenceOutputInsertion,
typename BothOutputInsertion = LeftOnlyOutputInsertion,
typename RightOnlyOutputInsertion = LeftOnlyOutputInsertion,
typename RangeLeft, typename RangeRight,
typename RangeLeftOnly, typename RangeBoth, typename RangeRightOnly,
typename Compare>
bool setSegregateOutputsExpected(RangeLeft const& left, RangeRight const& right,
                      RangeLeftOnly const& expectedLeftOnly, RangeBoth const& expectedBoth, RangeRightOnly const& expectedRightOnly, Compare compare)
{
    RangeLeftOnly leftOnly;
    RangeBoth both;
    RangeRightOnly rightOnly;
    
    fluent::set_segregate(left, right, LeftOnlyOutputInsertion()(leftOnly), BothOutputInsertion()(both), RightOnlyOutputInsertion()(rightOnly), compare);
    
    return ranges::equal(leftOnly, expectedLeftOnly)
    && ranges::equal(both, expectedBoth)
    && ranges::equal(rightOnly, expectedRightOnly);
}

TEST_CASE("set_segregate - left bigger")
{
    std::vector<int> left = {1, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 5, 6, 7};
    
    std::vector<int> expectedLeftOnly = {1, 2, 9};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 6};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - right bigger")
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {0, 3, 4, 5, 6, 7, 9};
    
    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {0, 4, 6, 9};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - left right are of same size")
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {3, 4, 5, 6, 7};
    
    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 6};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - take left in both")
{
    std::vector<int> left = {1, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 5, 6, 7};
    
    std::vector<int> expectedLeftOnly = {1, 2, 9};
    std::vector<int> expectedBoth = { 3, 5, 7 };
    std::vector<int> expectedRightOnly = {4, 6};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - left empty")
{
    std::vector<int> left = {};
    std::vector<int> right = {3, 4, 5, 6, 7};
    
    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {3, 4, 5, 6, 7};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - right empty")
{
    std::vector<int> left = {3, 4, 5, 6, 7};
    std::vector<int> right = {};
    
    std::vector<int> expectedLeftOnly = {3, 4, 5, 6, 7};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - all empty")
{
    std::vector<int> left = {};
    std::vector<int> right = {};
    
    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - identical elements")
{
    std::vector<int> left = {1, 2, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 4, 5, 5, 6, 7};
    
    std::vector<int> expectedLeftOnly = {1, 2, 2, 9};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 4, 5, 6};
    
    REQUIRE(setSegregateOutputsExpected(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - test map")
{
    std::map<int, std::string> left = {{1, "a"}, {2, "b"}, {3, "c"}, {5, "e"}, {7, "g"}, {9, "i"}};
    std::map<int, std::string> right = {{3, "c"}, {4, "d"}, {5, "e"}, {6, "f"},  {7, "g"}};
    
    std::map<int, std::string> expectedLeftOnly = {{1, "a"}, {2, "b"}, {9, "i"}};
    
    std::vector<
    std::pair<
    std::pair<int, std::string>,
    std::pair<int, std::string>
    >
    >
    expectedBoth = {std::make_pair(std::make_pair(3, "c"), std::make_pair(3, "c")),
        std::make_pair(std::make_pair(5, "e"), std::make_pair(5, "e")),
        std::make_pair(std::make_pair(7, "g"), std::make_pair(7, "g"))};
    
    std::map<int, std::string> expectedRightOnly = {{4, "d"}, {6, "f"}};
    
    REQUIRE(setSegregateOutputsExpected<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - test set")
{
    std::set<int> left = {1, 2, 3, 5, 7, 9};
    std::set<int> right = {3, 4, 5, 6, 7};
    
    std::set<int> expectedLeftOnly = {1, 2, 9};
    std::set<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::set<int> expectedRightOnly = {4, 6};
    
    REQUIRE(setSegregateOutputsExpected<AssociativeOutputInsertion>(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly));
}

TEST_CASE("set_segregate - compare on keys")
{
    std::map<int, std::string> left = {{1, "a"}, {2, "b"}, {3, "c1"}, {5, "e1"}, {7, "g1"}, {9, "i"}};
    std::map<int, std::string> right = {{3, "c2"}, {4, "d"}, {5, "e2"}, {6, "f"},  {7, "g2"}};
    
    std::map<int, std::string> expectedLeftOnly = {{1, "a"}, {2, "b"}, {9, "i"}};
    
    std::vector<
    std::pair<
    std::pair<int, std::string>,
    std::pair<int, std::string>
    >
    >
    expectedBoth = {std::make_pair(std::make_pair(3, "c1"), std::make_pair(3, "c2")),
        std::make_pair(std::make_pair(5, "e1"), std::make_pair(5, "e2")),
        std::make_pair(std::make_pair(7, "g1"), std::make_pair(7, "g2"))};
    
    std::map<int, std::string> expectedRightOnly = {{4, "d"}, {6, "f"}};
    
    REQUIRE(setSegregateOutputsExpected<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>
    (left, right, expectedLeftOnly, expectedBoth, expectedRightOnly,
     [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2) {return p1.first < p2.first;}));
}

TEST_CASE("set_segregate - compare on keys keep left")
{
    std::map<int, std::string> left = {{1, "a"}, {2, "b"}, {3, "c1"}, {5, "e1"}, {7, "g1"}, {9, "i"}};
    std::map<int, std::string> right = {{3, "c2"}, {4, "d"}, {5, "e2"}, {6, "f"},  {7, "g2"}};
    
    std::map<int, std::string> expectedLeftOnly = {{1, "a"}, {2, "b"}, {9, "i"}};
    
    std::vector<
    std::pair<int, std::string>
    >
    expectedBoth = {std::make_pair(3, "c1"),
        std::make_pair(5, "e1"),
        std::make_pair(7, "g1")};
    
    std::map<int, std::string> expectedRightOnly = {{4, "d"}, {6, "f"}};
    
    REQUIRE(setSegregateOutputsExpected<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>
    (left, right, expectedLeftOnly, expectedBoth, expectedRightOnly,
     [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2) {return p1.first < p2.first;}));
}


