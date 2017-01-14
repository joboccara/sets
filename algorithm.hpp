#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <algorithm>

namespace ranges
{

template <typename Range1, typename Range2>
bool equal(Range1 const& range1, Range2 const& range2)
{
    return std::equal(range1.begin(), range1.end(), range2.begin(), range2.end());
}

}

#endif

