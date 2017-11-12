#include "algorithm.hpp"
#include "set_aggregate.hpp"
#include "set_segregate.hpp"
#include "set_logical_operation.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <typeinfo>
#include <utility>
#include <vector>

namespace 
{
namespace test
{

template <typename T>
T tee(T const& value)
{
    std::cout << value << std::endl;
    return value;
}

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
bool testSetSegregate(RangeLeft const& left, RangeRight const& right,
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
bool testSetSegregate(RangeLeft const& left, RangeRight const& right,
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

bool leftBigger()
{
    std::vector<int> left = {1, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {1, 2, 9};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 6};
    
    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool rightBigger()
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {0, 3, 4, 5, 6, 7, 9};

    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {0, 4, 6, 9};
    
    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool leftRightEqualSize()
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 6};
    
    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool takeLeftInBoth()
{
    std::vector<int> left = {1, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {1, 2, 9};
    std::vector<int> expectedBoth = { 3, 5, 7 };
    std::vector<int> expectedRightOnly = {4, 6};
    
    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool leftEmpty()
{
    std::vector<int> left = {};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {3, 4, 5, 6, 7};    

    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool rightEmpty()
{
    std::vector<int> left = {3, 4, 5, 6, 7};
    std::vector<int> right = {};

    std::vector<int> expectedLeftOnly = {3, 4, 5, 6, 7};    
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};

    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool allEmpty()
{
    std::vector<int> left = {};
    std::vector<int> right = {};

    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};

    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool identicalElements()
{
    std::vector<int> left = {1, 2, 2, 3, 5, 7, 9};
    std::vector<int> right = {3, 4, 4, 5, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {1, 2, 2, 9};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 4, 5, 6};
    
    return testSetSegregate(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool testMap()
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
    
    return testSetSegregate<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool compareOnKeys()
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
    
    return testSetSegregate<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>
            (left, right, expectedLeftOnly, expectedBoth, expectedRightOnly,
            [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2) {return p1.first < p2.first;});
}

bool compareOnKeysKeepLeft()
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
    
    return testSetSegregate<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>
            (left, right, expectedLeftOnly, expectedBoth, expectedRightOnly,
            [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2) {return p1.first < p2.first;});
}

bool testSet()
{
    std::set<int> left = {1, 2, 3, 5, 7, 9};
    std::set<int> right = {3, 4, 5, 6, 7};

    std::set<int> expectedLeftOnly = {1, 2, 9};
    std::set<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::set<int> expectedRightOnly = {4, 6};
    
    return testSetSegregate<AssociativeOutputInsertion>(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> const& p)
{
    os << p.first << '-' << p.second;
    return os;
}

bool testSetAggregate()
{
    std::map<int, std::string> left = {{1, "a"}, {2, "b"}, {3, "c1"}, {5, "e1"}, {7, "g1"}, {9, "i"}};
    std::map<int, std::string> right = {{3, "c2"}, {4, "d"}, {5, "e2"}, {6, "f"},  {7, "g2"}};

    std::vector<std::pair<int, std::string>> expectedPermutation = {{1, "a"}, {2, "b"}, {3, "c1c2"}, {4, "d"}, {5, "e1e2"}, {6, "f"}, {7, "g1g2"}, {9, "i"}};
    std::vector<std::pair<int, std::string>> results;
    fluent::set_aggregate(left, right, std::back_inserter(results),
        [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2){ return p1.first < p2.first; },
        [](std::pair<const int, std::string> const& p1, std::pair<const int, std::string> const& p2){ return std::make_pair(p1.first, p1.second + p2.second); });

    return ranges::is_permutation(results, expectedPermutation);
}
    
bool testSetLogicalOperation()
{
    std::set<int> left = {1, 2, 3, 5, 7, 9};
    std::set<int> right = {2, 4, 5, 6, 10};

    std::vector<int> expected;
    std::vector<int> results;

    // none
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return false;});
    if (!ranges::equal(results, expected)) return false;

    // and
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft && inRight;});
    std::set_intersection(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
    if (!ranges::equal(results, expected)) return false;

    // left only
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft && !inRight;});
    std::set_difference(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
    if (!ranges::equal(results, expected)) return false;

    // left
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft;});
    expected.assign(begin(left), end(left));
    if (!ranges::equal(results, expected)) return false;
    
    // right only
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return !inLeft && inRight;});
    std::set_difference(begin(right), end(right), begin(left), end(left), std::back_inserter(expected));
    if (!ranges::equal(results, expected)) return false;
    
    // right
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inRight;});
    expected.assign(begin(right), end(right));
    if (!ranges::equal(results, expected)) return false;
    
    // xor
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft ^ inRight;});
    std::set_symmetric_difference(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
    if (!ranges::equal(results, expected)) return false;
    
    // or
    results.clear(); expected.clear();
    fluent::set_logical_operation(left, right, std::back_inserter(results), [](bool inLeft, bool inRight){ return inLeft || inRight;});
    std::set_union(begin(left), end(left), begin(right), end(right), std::back_inserter(expected));
    if (!ranges::equal(results, expected)) return false;

    return true;
}

bool vectorUnderlyingType()
{
    return typeid(range_underlying_type<std::vector<std::string>>) == typeid(std::string);
}

bool mapUnderlyingType()
{
    return typeid(range_underlying_type<std::map<int, std::string>>) == typeid(std::pair<const int, std::string>);
}

bool vectorIteratorUnderlyingType()
{
    return typeid(iterator_underlying_type<std::vector<std::string>::iterator>) == typeid(std::string);
}

bool mapIteratorUnderlyingType()
{
    return typeid(iterator_underlying_type<std::map<int, std::string>::iterator>) == typeid(std::pair<const int, std::string>);
}

bool backInserterUnderlyingType()
{
    return typeid(iterator_underlying_type<std::back_insert_iterator<std::vector<std::string>>>) == typeid(std::string);
}

bool inserterUnderlyingType()
{
    return typeid(iterator_underlying_type<std::insert_iterator<std::set<std::string>>>) == typeid(std::string);
}

bool isPairPairInt()
{
    return is_pair<std::pair<int, int>>::value == true;
}

bool isPairInt()
{
    return is_pair<int>::value == false;
}

bool pairFirstType()
{
    return typeid(get_pair_first<std::pair<int, std::string>>) == typeid(int);
}

bool pairSecondType()
{
    return typeid(get_pair_second<std::pair<int, std::string>>) == typeid(std::string);
}

bool bothContainsLeftRight()
{
    return BothContainsLeftAndRight
           <
               std::vector < std::pair<int, std::string> >::iterator,
               std::vector < int >,
               std::set < std::string >
           >::value == true
           &&
           BothContainsLeftAndRight
           <
               std::vector < std::pair<const int, const std::string> >::iterator,
               std::vector < int >,
               std::set < std::string >
           >::value == true
           &&
           BothContainsLeftAndRight
           <
               std::set < std::pair<int, std::string> >::iterator,
               std::set < int >,
               std::vector < std::string >
           >::value == true
           &&
           BothContainsLeftAndRight
           <
               std::vector < int >::iterator,
               std::set < int >,
               std::vector < int >
           >::value == false;
           
}

template <typename TestFunction>
bool launchTest(std::string const& testName, TestFunction testFunction)
{
    const bool success = testFunction();
    if (!success)
        std::cout << "Test - " << testName << ": FAILED\n";
    return success;
}

void launchTests()
{
    bool success = true;
    success &= launchTest("Left bigger", leftBigger);
    success &= launchTest("Right bigger", rightBigger);
    success &= launchTest("Left right equal size", leftRightEqualSize);
    success &= launchTest("Take left in both", takeLeftInBoth);
    success &= launchTest("Left empty", leftEmpty);
    success &= launchTest("Right empty", rightEmpty);
    success &= launchTest("All empty", allEmpty);
    success &= launchTest("Identical elements", identicalElements);
    success &= launchTest("Map", testMap);
    success &= launchTest("Set", testSet);
    success &= launchTest("Compare on keys", compareOnKeys);
    success &= launchTest("Compare on keys keep left", compareOnKeysKeepLeft);

    success &= launchTest("Set aggregate", testSetAggregate);
    
    success &= launchTest("set_logical_operation", testSetLogicalOperation);

    success &= launchTest("Vector underlying type", vectorUnderlyingType);
    success &= launchTest("Map underlying type", mapUnderlyingType);
    success &= launchTest("Vector iterator underlying type", vectorIteratorUnderlyingType);
    success &= launchTest("Map iterator underlying type", mapIteratorUnderlyingType);
    success &= launchTest("Back inserter underlying type", backInserterUnderlyingType);
    success &= launchTest("Inserter underlying type", inserterUnderlyingType);
    success &= launchTest("Is pair std::pair<int, int>", isPairPairInt);
    success &= launchTest("Is pair int", isPairInt);
    success &= launchTest("Pair 1st type", pairFirstType);
    success &= launchTest("Pair 2st type", pairSecondType);
    success &= launchTest("Both contains left and right", bothContainsLeftRight);
    
    if (success)
        std::cout << "All tests PASSED\n";
}

}
}

int main()
{
    test::launchTests();
}

