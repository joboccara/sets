#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>

namespace ranges
{
    
template <typename Range>
auto raangeSize(Range const& range) -> decltype(std::distance(begin(range), end(range)))
{
    return std::distance(begin(range), end(range));
}
    
template <typename Range1, typename Range2>
bool equal(Range1 const& range1, Range2 const& range2)
{
    if (size(range1) != size(range2)) return false;
    
    return std::equal(range1.begin(), range1.end(), range2.begin());
}

template <typename Range1, typename Range2>
bool is_permutation(Range1 const& range1, Range2 const& range2)
{
    if (size(range1) != size(range2)) return false;
    
    return std::is_permutation(range1.begin(), range1.end(), range2.begin());
}


}

#endif

