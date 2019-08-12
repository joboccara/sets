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
    }};

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

template<typename Action> struct FirstLessThanSecond {};
template<typename Action> struct SecondLessThanFirst {};
template<typename Action> struct BothEquivalent {};
template<typename Action> struct FinishedTraversal {};

template<typename SetA, typename SetB, typename Compare,
         typename PredicateAlessThanB,
         typename PredicateBlessThanA,
         typename PredicateBothEquivalent,
         typename PredicateFinishedTraversal>
bool set_bool_information(SetA&& setA,
                          SetB&& setB,
                          Compare&& comp,
                          FirstLessThanSecond<PredicateAlessThanB>,
                          SecondLessThanFirst<PredicateBlessThanA>,
                          BothEquivalent<PredicateBothEquivalent>,
                          FinishedTraversal<PredicateFinishedTraversal>)
{
    auto it1 = begin(setA);
    auto it2 = begin(setB);
    
    while (it1 != end(setA) && it2 != end(setB))
    {
        if (comp(*it1, *it2))
        {
            if (std::is_same<PredicateAlessThanB, MoveOn>::value)
            {
                ++it1;
            }
            else
            {
                return PredicateAlessThanB::_();
            }
        }
        else if (comp(*it2, *it1))
        {
            if (std::is_same<PredicateBlessThanA, MoveOn>::value)
            {
                ++it2;
            }
            else
            {
                return PredicateBlessThanA::_();
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

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<MoveOn>{},
                                SecondLessThanFirst<ReturnFalse>{},
                                BothEquivalent<MoveOn>{},
                                FinishedTraversal<ReturnHasReachedEndOfFirst>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of_other(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<MoveOn>{},
                                SecondLessThanFirst<ReturnFalse>{},
                                BothEquivalent<MoveOn>{},
                                FinishedTraversal<ReturnTrue>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool set_share_element(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<MoveOn>{},
                                SecondLessThanFirst<MoveOn>{},
                                BothEquivalent<ReturnTrue>{},
                                FinishedTraversal<ReturnFalse>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool includes(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<MoveOn>{},
                                SecondLessThanFirst<ReturnFalse>{},
                                BothEquivalent<MoveOn>{},
                                FinishedTraversal<ReturnHasReachedEndOfSecond>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool disjoint(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return !set_share_element(FWD(set1), FWD(set2), comp);
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool equivalent(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<ReturnFalse>{},
                                SecondLessThanFirst<ReturnFalse>{},
                                BothEquivalent<MoveOn>{},
                                FinishedTraversal<ReturnHasReachedEndOfBoth>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_before(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set2) == end(set2)) return false;
    
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<MoveOn>{},
                                SecondLessThanFirst<ReturnFalse>{},
                                BothEquivalent<ReturnFalse>{},
                                FinishedTraversal<ReturnTrue>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_after(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set1) == end(set1)) return false;
    
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<ReturnFalse>{},
                                SecondLessThanFirst<MoveOn>{},
                                BothEquivalent<ReturnFalse>{},
                                FinishedTraversal<ReturnTrue>{});
}

#endif /* SET_BOOL_ALGORITHMS_HPP */
