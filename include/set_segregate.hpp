#ifndef SET_SEGREGATE_HPP
#define SET_SEGREGATE_HPP

#include "metaprog.hpp"

namespace fluent
{
namespace details
{
    
template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare, class AddToBoth>
void set_segregate_impl(LeftRange const& leftRange, RightRange const& rightRange,
                         OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                         Compare comp, AddToBoth addToBoth)
{
    auto left = leftRange.begin();
    auto right = rightRange.begin();
    while (left != leftRange.end() && right != rightRange.end())
    {
        if (comp(*left, *right))
        {
            *leftOnly++ = *left++;
        }
        else if (comp(*right, *left))
        {
            *rightOnly++ = *right++;
        }
        else
        {
            *both++ = addToBoth(*left++, *right++);
        }
    }
    std::copy(left, end(leftRange), leftOnly);
    std::copy(right, end(rightRange), rightOnly);
}

struct AddPairToBoth
{
    template<typename LeftElement, typename RightElement>
    std::pair<LeftElement, RightElement> operator()(LeftElement const& leftElement, RightElement const& rightElement)
    {
        return std::make_pair(leftElement, rightElement);
    }
};

struct AddLeftToBoth
{
    template<typename LeftElement, typename RightElement>
    LeftElement operator()(LeftElement const& leftElement, RightElement const&)
    {
        return leftElement;
    }
};

template<bool bothContainsLeftAndRight>
struct Set_segregate_impl
{
    template<class LeftRange, class RightRange,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(LeftRange const& leftRange, RightRange const& rightRange,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_segregate_impl(leftRange, rightRange, leftOnly, both, rightOnly, comp, AddPairToBoth());
    }
};

template<>
struct Set_segregate_impl<false>
{
    template<class LeftRange, class RightRange,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(LeftRange const& leftRange, RightRange const& rightRange,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_segregate_impl(leftRange, rightRange, leftOnly, both, rightOnly, comp, AddLeftToBoth());
    }
};
} // namespace details

template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
void set_segregate(LeftRange const& leftRange, RightRange const& rightRange,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                    Compare comp)
{
    details::Set_segregate_impl<BothContainsLeftAndRight<OutputItBoth, LeftRange, RightRange>::value>()(leftRange, rightRange, leftOnly, both, rightOnly, comp);
}

template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight>
void set_segregate(LeftRange const& leftRange, RightRange const& rightRange,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly)
{
    set_segregate(leftRange, rightRange, leftOnly, both, rightOnly, std::less<typename std::iterator_traits<typename LeftRange::const_iterator>::reference>());
}

} // namespace fluent

#endif

