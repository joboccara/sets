//
//  set_match.hpp
//  Sets
//
//  Created by Jonathan Boccara on 14/11/2017.
//  Copyright © 2017 Jonathan Boccara. All rights reserved.
//

#ifndef set_match_h
#define set_match_h

#include "dead_end_iterator.hpp"
#include "set_segregate.hpp"

namespace fluent
{

template<typename Range1, typename Range2, typename OutputIterator, typename Comparator>
OutputIterator set_match(Range1 const& range1, Range2 const& range2, OutputIterator out, Comparator comparator)
{
    set_segregate(range1, range2, dead_end_iterator(), out, dead_end_iterator(), comparator);
    return out;
}

} // namespace fluent

#endif /* set_match_h */
