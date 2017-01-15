#ifndef SET_SEGGREGATE_HPP
#define SET_SEGGREGATE_HPP

#include "metaprog.hpp"

template<class Range1, class Range2,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare, class AddToBoth>
void set_seggregate_impl(Range1 const& range1, Range2 const& range2,
                         OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                         Compare comp, AddToBoth addToBoth)
{
    auto it1 = range1.begin();
    auto it2 = range2.begin();
    while (it1 != range1.end())
    {
        if (it2 == range2.end())
        {
            std::copy(it1, range1.end(), leftOnly);
            return;
        }
 
        if (comp(*it1, *it2))
        {
            *leftOnly++ = *it1++;
        }
        else
        {
            if (!comp(*it2, *it1))
            {
                *both++ = addToBoth(*it1++, *it2++);
            }
            else
            {
                *rightOnly++ = *it2++;
            }
        }
    }
    std::copy(it2, range2.end(), rightOnly);
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
    template<class Range1, class Range2,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(Range1 const& range1, Range2 const& range2,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_seggregate_impl(range1, range2, leftOnly, both, rightOnly, comp, AddPairToBoth());
    }
};

template<>
struct Set_seggregate_impl<false>
{
    template<class Range1, class Range2,
             class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
    void operator()(Range1 const& range1, Range2 const& range2,
                           OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                           Compare comp)
    {
        set_seggregate_impl(range1, range2, leftOnly, both, rightOnly, comp, AddLeftToBoth());
    }
};


template<class Range1, class Range2,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
void set_seggregate(Range1 const& range1, Range2 const& range2,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                    Compare comp)
{
    Set_seggregate_impl<BothContainsLeftAndRight<OutputItBoth, Range1, Range2>::value>()(range1, range2, leftOnly, both, rightOnly, comp);
}

template<class Range1, class Range2,
         class OutputItLeft, class OutputItBoth, class OutputItRight>
void set_seggregate( Range1 const& range1, Range2 const& range2,
                     OutputItLeft rightOnly, OutputItBoth both, OutputItRight leftOnly)
{
    set_seggregate(range1, range2, rightOnly, both, leftOnly, std::less<typename std::iterator_traits<typename Range1::const_iterator>::reference>());
}

#endif

