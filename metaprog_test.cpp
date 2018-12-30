#include "catch.hpp"

#include "metaprog.hpp"

#include <iterator>
#include <map>
#include <set>
#include <utility>

TEST_CASE("vector underlying type")
{
    REQUIRE(typeid(range_underlying_type<std::vector<std::string>>) == typeid(std::string));
}

TEST_CASE("map underlying type")
{
    REQUIRE(typeid(range_underlying_type<std::map<int, std::string>>) == typeid(std::pair<const int, std::string>));
}

TEST_CASE("vector iterator underlying type")
{
    REQUIRE(typeid(iterator_underlying_type<std::vector<std::string>::iterator>) == typeid(std::string));
}

TEST_CASE("map iterator underlying type")
{
    REQUIRE(typeid(iterator_underlying_type<std::map<int, std::string>::iterator>) == typeid(std::pair<const int, std::string>));
}

TEST_CASE("back inserter underlying type")
{
    REQUIRE(typeid(iterator_underlying_type<std::back_insert_iterator<std::vector<std::string>>>) == typeid(std::string));
}

TEST_CASE("inserter underlying type")
{
    REQUIRE(typeid(iterator_underlying_type<std::insert_iterator<std::set<std::string>>>) == typeid(std::string));
}

TEST_CASE("pair of int is a pair")
{
    REQUIRE(is_pair<std::pair<int, int>>::value == true);
}

TEST_CASE("int is not a pair")
{
    REQUIRE(!is_pair<int>::value);
}

TEST_CASE("both contains left and right")
{
    REQUIRE(BothContainsLeftAndRight
            <
                std::vector < std::pair<int, std::string> >::iterator,
                std::vector < int >,
                std::set < std::string >
            >::value);

    REQUIRE(BothContainsLeftAndRight
            <
                std::vector < std::pair<const int, const std::string> >::iterator,
                std::vector < int >,
                std::set < std::string >
            >::value);

    REQUIRE(BothContainsLeftAndRight
            <
                std::set < std::pair<int, std::string> >::iterator,
                std::set < int >,
                std::vector < std::string >
            >::value);

    REQUIRE(!BothContainsLeftAndRight
            <
                std::vector < int >::iterator,
                std::set < int >,
                std::vector < int >
            >::value);
}

