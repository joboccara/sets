#ifndef SET_BOOL_ALGORITHMS_HPP
#define SET_BOOL_ALGORITHMS_HPP

#include "detail/FWD.hpp"
#include <functional>
#include <type_traits>

struct MoveOn
{
    static bool _() { assert(false); }
};

struct ReturnTrue
{
    static bool _() { return true; }

    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    static bool _(Iterator1&&, End1&&, Iterator2&&, End2&&)
    {
        return true;
    }
};

struct ReturnFalse
{
    static bool _() { return false; }

    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    static bool _(Iterator1&&, End1&&, Iterator2&&, End2&&)
    {
        return false;
    }
};

struct ReturnHasReachedEndOfFirst
{
    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    static bool _(Iterator1&& it1, End1&& end1, Iterator2&&, End2&&)
    {
        return it1 == end1;
    }
};

struct ReturnHasReachedEndOfSecond
{
    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    static bool _(Iterator1&&, End1&&, Iterator2&& it2, End2&& end2)
    {
        return it2 == end2;
    }
};

struct ReturnHasReachedEndOfBoth
{
    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    static bool _(Iterator1&& it1, End1&& end1, Iterator2&& it2, End2&& end2)
    {
        return it1 == end1 && it2 == end2;
    }
};

template<typename PredicateFirstLessThanSecondPolicy,
         typename PredicateSecondLessThanFirstPolicy,
         typename PredicateBothEquivalentPolicy,
         typename PredicateFinishedTraversalPolicy,
         typename SetA, typename SetB, typename Compare>
bool set_bool_information(SetA&& setA, SetB&& setB, Compare&& comp)
{
    using PredicateFirstLessThanSecond = typename PredicateFirstLessThanSecondPolicy::Predicate;
    using PredicateSecondLessThanFirst = typename PredicateSecondLessThanFirstPolicy::Predicate;
    using PredicateBothEquivalent = typename PredicateBothEquivalentPolicy::Predicate;
    using PredicateFinishedTraversal = typename PredicateFinishedTraversalPolicy::Predicate;
    auto it1 = begin(setA);
    auto it2 = begin(setB);
    
    while (it1 != end(setA) && it2 != end(setB))
    {
        if (comp(*it1, *it2))
        {
            if (std::is_same<PredicateFirstLessThanSecond, MoveOn>::value)
            {
                ++it1;
            }
            else
            {
                return PredicateFirstLessThanSecond::_();
            }
        }
        else if (comp(*it2, *it1))
        {
            if (std::is_same<PredicateSecondLessThanFirst, MoveOn>::value)
            {
                ++it2;
            }
            else
            {
                return PredicateSecondLessThanFirst::_();
            }
        }
        else
        {
            if (std::is_same<PredicateBothEquivalent, MoveOn>::value)
            {
                ++it1;
                ++it2;
            }
            else
            {
                return PredicateBothEquivalent::_();
            }
        }
    }
    return PredicateFinishedTraversal::_(it1, end(setA), it2, end(setB));
}

template<typename T>
struct FirstLessThanSecond
{
    using Predicate = T;
};

template<typename T>
struct SecondLessThanFirst
{
    using Predicate = T;
};

template<typename T>
struct BothEquivalent
{
    using Predicate = T;
};

template<typename T>
struct FinishedTraversal
{
    using Predicate = T;
};

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information<SecondLessThanFirst<ReturnFalse>,
                                 FirstLessThanSecond<ReturnFalse>,
                                 BothEquivalent<MoveOn>,
                                 FinishedTraversal<ReturnHasReachedEndOfFirst>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of_other(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information<SecondLessThanFirst<MoveOn>,
                                FirstLessThanSecond<ReturnFalse>,
                                BothEquivalent<MoveOn>,
                                FinishedTraversal<ReturnTrue>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool set_share_element(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{

    return set_bool_information<SecondLessThanFirst<MoveOn>,
                                FirstLessThanSecond<MoveOn>,
                                BothEquivalent<ReturnTrue>,
                                FinishedTraversal<ReturnFalse>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool includes(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information<SecondLessThanFirst<MoveOn>,
                                FirstLessThanSecond<ReturnFalse>,
                                BothEquivalent<MoveOn>,
                                FinishedTraversal<ReturnHasReachedEndOfSecond>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool disjoint(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return !set_share_element(FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool equivalent(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information<SecondLessThanFirst<ReturnFalse>,
                                FirstLessThanSecond<ReturnFalse>,
                                BothEquivalent<MoveOn>,
                                FinishedTraversal<ReturnHasReachedEndOfBoth>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_before(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set2) == end(set2)) return false;
    
    return set_bool_information<SecondLessThanFirst<MoveOn>,
                                FirstLessThanSecond<ReturnFalse>,
                                BothEquivalent<ReturnFalse>,
                                FinishedTraversal<ReturnTrue>>
                                (FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_after(Set1&& set1, Set2&& set2, Compare comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set1) == end(set1)) return false;
    
    return set_bool_information<SecondLessThanFirst<ReturnFalse>,
                                FirstLessThanSecond<MoveOn>,
                                BothEquivalent<ReturnFalse>,
                                FinishedTraversal<ReturnTrue>>
                                (FWD(set1), FWD(set2), comp);
}

#endif /* SET_BOOL_ALGORITHMS_HPP */
