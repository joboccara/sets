#ifndef SET_SEGGREGATE_HPP
#define SET_SEGGREGATE_HPP

#include "metaprog.hpp"

namespace fluent
{
namespace details
{
    
template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare, class AddToBoth>
void set_seggregate_impl(LeftRange const& leftRange, RightRange const& rightRange,
                         OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                         Compare comp, AddToBoth addToBoth)
{
    auto itLeft = leftRange.begin();
    auto itRight = rightRange.begin();
    while (itLeft != leftRange.end())
    {
        if (itRight == rightRange.end())
        {
            std::copy(itLeft, leftRange.end(), leftOnly);
            return;
        }
 
        if (comp(*itLeft, *itRight))
        {
            *leftOnly++ = *itLeft++;
        }
        else
        {
            if (!comp(*itRight, *itLeft))
            {
                *both++ = addToBoth(*itLeft++, *itRight++);
            }
            else
            {
                *rightOnly++ = *itRight++;
            }
        }
    }
    std::copy(itRight, rightRange.end(), rightOnly);
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
struct Set_seggregate_impl
{
    template<class LeftRange, class RightRange,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(LeftRange const& leftRange, RightRange const& rightRange,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_seggregate_impl(leftRange, rightRange, leftOnly, both, rightOnly, comp, AddPairToBoth());
    }
};

template<>
struct Set_seggregate_impl<false>
{
    template<class LeftRange, class RightRange,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(LeftRange const& leftRange, RightRange const& rightRange,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_seggregate_impl(leftRange, rightRange, leftOnly, both, rightOnly, comp, AddLeftToBoth());
    }
};
} // namespace details

template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
void set_seggregate(LeftRange const& leftRange, RightRange const& rightRange,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                    Compare comp)
{
    details::Set_seggregate_impl<BothContainsLeftAndRight<OutputItBoth, LeftRange, RightRange>::value>()(leftRange, rightRange, leftOnly, both, rightOnly, comp);
}

template<class LeftRange, class RightRange,
         class OutputItLeft, class OutputItBoth, class OutputItRight>
void set_seggregate(LeftRange const& leftRange, RightRange const& rightRange,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly)
{
    set_seggregate(leftRange, rightRange, leftOnly, both, rightOnly, std::less<typename std::iterator_traits<typename LeftRange::const_iterator>::reference>());
}

} // namespace fluent

#endif

