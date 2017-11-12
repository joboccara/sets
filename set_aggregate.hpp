#ifndef SET_AGGREGATE_HPP
#define SET_AGGREGATE_HPP

#include <algorithm>
#include <iterator>
#include "set_seggregate.hpp"
#include <vector>

namespace fluent
{

template<typename Range1, typename Range2, typename OutputIterator, typename Compare, typename Function>
OutputIterator set_aggregate(Range1 const& range1, Range2 const& range2,
                   OutputIterator output, Compare compare, Function aggregator)
{
    using value_type1 = typename std::remove_reference<decltype(*range1.begin())>::type;
    using value_type2 = typename std::remove_reference<decltype(*range2.begin())>::type;

    std::vector<std::pair<value_type1, value_type2>> elementsInBoth;
    set_seggregate(range1, range2, output, std::back_inserter(elementsInBoth), output, compare);
    return std::transform(elementsInBoth.begin(), elementsInBoth.end(),
                          output,
                          [aggregator](std::pair<value_type1, value_type2> const& p){return aggregator(p.first, p.second);});
}
    
} // namespace fluent
#endif

