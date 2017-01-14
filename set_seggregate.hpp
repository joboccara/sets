#ifndef SET_SEGGREGATE_HPP
#define SET_SEGGREGATE_HPP

template<class Range1, class Range2,
         class OutputItLeft, class OutputItBoth, class OutputItRight, class Compare>
void set_seggregate(Range1 const& range1, Range2 const& range2,
                    OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly,
                    Compare comp)
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
                *both++ = std::make_pair(*it1++, *it2++);
            }
            else
            {
                *rightOnly++ = *it2++;
            }
        }
    }
    std::copy(it2, range2.end(), rightOnly);
}

template<class Range1, class Range2,
         class OutputItLeft, class OutputItBoth, class OutputItRight>
void set_seggregate( Range1 const& range1, Range2 const& range2,
                     OutputItLeft rightOnly, OutputItBoth both, OutputItRight leftOnly)
{
    set_seggregate(range1, range2, rightOnly, both, leftOnly, std::less<typename std::iterator_traits<typename Range1::const_iterator>::reference>());
}

#endif

