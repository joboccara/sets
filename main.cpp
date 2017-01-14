#include "algorithm.hpp"
#include "set_seggregate.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
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

template<class RangeLeft, class RangeRight, class RangeLeftOnly, class RangeBoth, class RangeRightOnly>
bool performTest(RangeLeft const& left, RangeRight const& right,
          RangeLeftOnly const& expectedLeftOnly, RangeBoth const& expectedBoth, RangeRightOnly const& expectedRightOnly)
{
    std::vector<int> leftOnly;
    std::vector<std::pair<int, int>> both;
    std::vector<int> rightOnly;
    
    set_seggregate(left, right, std::back_inserter(leftOnly), std::back_inserter(both), std::back_inserter(rightOnly));

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
}

}

int main()
{
    test::launchTests();
}

