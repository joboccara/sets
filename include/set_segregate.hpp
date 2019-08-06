#ifndef SET_SEGREGATE_HPP
#define SET_SEGREGATE_HPP

#include "FWD.hpp"
#include "metaprog.hpp"
#include <type_traits>

namespace fluent
{
namespace details
{
    
template<class SetA, class SetB,
         class OutputOnlyA, class OutputBoth, class OutputOnlyB, class Compare, class AddToBoth>
void set_segregate_impl(SetA&& setA, SetB&& setB,
                         OutputOnlyA&& onlyA, OutputBoth&& both, OutputOnlyB&& onlyB,
                         Compare comp, AddToBoth addToBoth)
{
    auto xA = setA.begin();
    auto xB = setB.begin();
    while (xA != setA.end() && xB != setB.end())
    {
        if (comp(*xA, *xB))
        {
            *onlyA++ = *xA++;
        }
        else if (comp(*xB, *xA))
        {
            *onlyB++ = *xB++;
        }
        else
        {
            *both++ = addToBoth(*xA++, *xB++);
        }
    }
    std::copy(xA, end(setA), onlyA);
    std::copy(xB, end(setB), onlyB);
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
    template<class SetA, class SetB,
    class OutputOnlyA, class OutputBoth, class OutputOnlyB, class Compare>
    static void _(SetA&& setA, SetB&& setB,
                  OutputOnlyA&& onlyA, OutputBoth&& both, OutputOnlyB&& onlyB,
                  Compare comp)
    {
        set_segregate_impl(FWD(setA), FWD(setB), FWD(onlyA), FWD(both), FWD(onlyB), comp, AddPairToBoth());
    }
};

template<>
struct Set_segregate_impl<false>
{
    template<class SetA, class SetB,
    class OutputOnlyA, class OutputBoth, class OutputOnlyB, class Compare>
    static void _(SetA&& setA, SetB&& setB,
                  OutputOnlyA&& onlyA, OutputBoth&& both, OutputOnlyB&& onlyB,
                  Compare comp)
    {
        set_segregate_impl(FWD(setA), FWD(setB), FWD(onlyA), FWD(both), FWD(onlyB), comp, AddLeftToBoth());
    }
};
} // namespace details

template<class SetA, class SetB,
class OutputOnlyA, class OutputBoth, class OutputOnlyB, class Compare>
void set_segregate(SetA&& setA, SetB&& setB,
                   OutputOnlyA&& onlyA, OutputBoth&& both, OutputOnlyB&& onlyB,
                   Compare comp)
{
    details::Set_segregate_impl<BothContainsLeftAndRight<typename std::remove_reference<OutputBoth>::type,
                                                         typename std::remove_reference<SetA>::type,
                                                         typename std::remove_reference<SetB>::type>::value>::_(FWD(setA), FWD(setB), FWD(onlyA), FWD(both), FWD(onlyB), comp);
}

template<class SetA, class SetB,
class OutputOnlyA, class OutputBoth, class OutputOnlyB>
void set_segregate(SetA&& setA, SetB&& setB,
                   OutputOnlyA&& onlyA, OutputBoth&& both, OutputOnlyB&& onlyB)
{
    set_segregate(FWD(setA), FWD(setB), FWD(onlyA), FWD(both), FWD(onlyB), std::less<typename std::iterator_traits<typename std::remove_reference<SetA>::type::const_iterator>::reference>());
}

} // namespace fluent

#endif

