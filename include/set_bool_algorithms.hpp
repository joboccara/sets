#ifndef SET_BOOL_ALGORITHMS_HPP
#define SET_BOOL_ALGORITHMS_HPP

#include "detail/FWD.hpp"
#include <functional>
#include <type_traits>

enum class BoolActionType
{
    MoveOn,
    True,
    False,
    HasReachedEndOfFirst,
    HasReachedEndOfSecond
};

template<BoolActionType boolActionType>
struct BoolAction;

template<>
struct BoolAction<BoolActionType::MoveOn>
{
    bool operator()() { assert(false); } // should be removed by using if constexpr at call site
};

template<>
struct BoolAction<BoolActionType::True>
{
    bool operator()() { return true; }

    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    bool operator()(Iterator1&&, End1&&, Iterator2&&, End2&&)
    {
        return true;
    }
};

template<>
struct BoolAction<BoolActionType::False>
{
    bool operator()() { return false; }

    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    bool operator()(Iterator1&&, End1&&, Iterator2&&, End2&&)
    {
        return false;
    }};

template<>
struct BoolAction<BoolActionType::HasReachedEndOfFirst>
{
    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    bool operator()(Iterator1&& it1, End1&& end1, Iterator2&&, End2&&)
    {
        return it1 == end1;
    }
};

template<>
struct BoolAction<BoolActionType::HasReachedEndOfSecond>
{
    template<typename Iterator1, typename End1, typename Iterator2, typename End2>
    bool operator()(Iterator1&&, End1&&, Iterator2&& it2, End2&& end2)
    {
        return it2 == end2;
    }
};

template<BoolActionType boolActionType> struct FirstLessThanSecond : BoolAction<boolActionType> {};
template<BoolActionType boolActionType> struct SecondLessThanFirst : BoolAction<boolActionType> {};
template<BoolActionType boolActionType> struct BothEquivalent : BoolAction<boolActionType> {};
template<BoolActionType boolActionType> struct FinishedTraversal : BoolAction<boolActionType> {};

template<typename Set1, typename Set2, typename Compare,
         BoolActionType boolAction1LessThan2,
         BoolActionType boolAction2LessThan1,
         BoolActionType boolActionBothEquivalent,
         BoolActionType boolActionFinishedTraversal>
bool set_bool_information(Set1&& set1,
                          Set2&& set2,
                          Compare&& comp,
                          FirstLessThanSecond<boolAction1LessThan2> resultFor1LessThan2,
                          SecondLessThanFirst<boolAction2LessThan1> resultFor2LessThan1,
                          BothEquivalent<boolActionBothEquivalent> resultForBothEquivalent,
                          FinishedTraversal<boolActionFinishedTraversal> resultForFinishedTraversal)
{
    auto it1 = begin(set1);
    auto it2 = begin(set2);
    
    while (it1 != end(set1) && it2 != end(set2))
    {
        if (comp(*it1, *it2))
        {
            if (boolAction1LessThan2 == BoolActionType::MoveOn)
            {
                ++it1;
            }
            else
            {
                return resultFor1LessThan2();
            }
        }
        else if (comp(*it2, *it1))
        {
            if (boolAction2LessThan1 == BoolActionType::MoveOn)
            {
                ++it2;
            }
            else
            {
                return resultFor2LessThan1();
            }
        }
        else
        {
            if (boolActionBothEquivalent == BoolActionType::MoveOn)
            {
                ++it1;
                ++it2;
            }
            else
            {
                return resultForBothEquivalent();
            }
        }
    }
    return resultForFinishedTraversal(it1, end(set1), it2, end(set2));
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::MoveOn>{},
                                SecondLessThanFirst<BoolActionType::False>{},
                                BothEquivalent<BoolActionType::MoveOn>{},
                                FinishedTraversal<BoolActionType::HasReachedEndOfFirst>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_prefix_of_other(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::MoveOn>{},
                                SecondLessThanFirst<BoolActionType::False>{},
                                BothEquivalent<BoolActionType::MoveOn>{},
                                FinishedTraversal<BoolActionType::True>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool set_share_element(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::MoveOn>{},
                                SecondLessThanFirst<BoolActionType::MoveOn>{},
                                BothEquivalent<BoolActionType::True>{},
                                FinishedTraversal<BoolActionType::False>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool includes(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::MoveOn>{},
                                SecondLessThanFirst<BoolActionType::False>{},
                                BothEquivalent<BoolActionType::MoveOn>{},
                                FinishedTraversal<BoolActionType::HasReachedEndOfSecond>{});
}


template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool disjoint(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    return !set_share_element(FWD(set1), FWD(set2), comp);
}


template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_before(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set2) == end(set2)) return false;
    
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::MoveOn>{},
                                SecondLessThanFirst<BoolActionType::False>{},
                                BothEquivalent<BoolActionType::False>{},
                                FinishedTraversal<BoolActionType::True>{});
}

template <typename Set1, typename Set2, typename Compare = std::less<typename std::remove_reference<Set1>::type::value_type>>
bool is_after(Set1&& set1, Set2&& set2, Compare&& comp = std::less<typename std::remove_reference<Set1>::type::value_type>{})
{
    if (begin(set1) == end(set1)) return false;
    
    return set_bool_information(FWD(set1),
                                FWD(set2),
                                comp,
                                FirstLessThanSecond<BoolActionType::False>{},
                                SecondLessThanFirst<BoolActionType::MoveOn>{},
                                BothEquivalent<BoolActionType::False>{},
                                FinishedTraversal<BoolActionType::True>{});
}

#endif /* SET_BOOL_ALGORITHMS_HPP */
