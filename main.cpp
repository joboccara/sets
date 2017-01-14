#include "algorithm.hpp"
#include "set_seggregate.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

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
    auto operator()(SequenceContainer& container)
    {
        return std::back_inserter(container);
    }
};

struct AssociativeOutputInsertion
{
    template<typename AssociativeContainer>
    auto operator()(AssociativeContainer& container)
    {
        return std::inserter(container, container.end());
    }
};

template<typename LeftOnlyOutputInsertion = SequenceOutputInsertion,
         typename BothOutputInsertion = SequenceOutputInsertion,
         typename RightOnlyOutputInsertion = SequenceOutputInsertion,
         typename RangeLeft, typename RangeRight,
         typename RangeLeftOnly, typename RangeBoth, typename RangeRightOnly>
bool performTest(RangeLeft const& left, RangeRight const& right,
                 RangeLeftOnly const& expectedLeftOnly, RangeBoth const& expectedBoth, RangeRightOnly const& expectedRightOnly)
{
    RangeLeftOnly leftOnly;
    RangeBoth both;
    RangeRightOnly rightOnly;
    
    set_seggregate(left, right, LeftOnlyOutputInsertion()(leftOnly), BothOutputInsertion()(both), RightOnlyOutputInsertion()(rightOnly));

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
    
    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool rightBigger()
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {0, 3, 4, 5, 6, 7, 9};

    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {0, 4, 6, 9};
    
    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool leftRightEqualSize()
{
    std::vector<int> left = {1, 2, 3, 5, 7};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {1, 2};
    std::vector<std::pair<int, int>> expectedBoth = {std::make_pair(3, 3), std::make_pair(5, 5), std::make_pair(7, 7)};
    std::vector<int> expectedRightOnly = {4, 6};
    
    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool leftEmpty()
{
    std::vector<int> left = {};
    std::vector<int> right = {3, 4, 5, 6, 7};

    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {3, 4, 5, 6, 7};    

    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool rightEmpty()
{
    std::vector<int> left = {3, 4, 5, 6, 7};
    std::vector<int> right = {};

    std::vector<int> expectedLeftOnly = {3, 4, 5, 6, 7};    
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};

    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

bool allEmpty()
{
    std::vector<int> left = {};
    std::vector<int> right = {};

    std::vector<int> expectedLeftOnly = {};
    std::vector<std::pair<int, int>> expectedBoth = {};
    std::vector<int> expectedRightOnly = {};

    return performTest(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
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
    
    return performTest<AssociativeOutputInsertion, SequenceOutputInsertion, AssociativeOutputInsertion>(left, right, expectedLeftOnly, expectedBoth, expectedRightOnly);
}

template <typename TestFunction>
void launchTest(std::string const& testName, TestFunction testFunction)
{
    std::cout << "Test - " << testName << ": " << (testFunction() ? "OK" : "FAILED") << std::endl;
}

void launchTests()
{
    launchTest("Left bigger", leftBigger);
    launchTest("Right bigger", rightBigger);
    launchTest("Left right equal size", leftRightEqualSize);
    launchTest("Left empty", leftEmpty);
    launchTest("Right empty", rightEmpty);
    launchTest("All empty", allEmpty);
    launchTest("Map", testMap);
}

}

int main()
{
    test::launchTests();
}

