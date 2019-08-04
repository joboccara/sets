//
//  set_logic_operation.h
//  Sets
//
//  Created by Jonathan Boccara on 10/11/2017.
//  Copyright © 2017 Jonathan Boccara. All rights reserved.
//

#ifndef set_logical_operation_hpp
#define set_logical_operation_hpp

#include "dead_end_iterator.hpp"
#include "set_segregate.hpp"

namespace fluent
{
template<typename SetA, typename SetB, typename OutputIterator, typename LogicalOperation>
OutputIterator set_logical_operation(SetA&& setA, SetB&& setB, OutputIterator out, LogicalOperation logicalOperation)
{
    auto const includeElementsInAOnly = logicalOperation(true, false);
    auto const includeElementsInBOnly = logicalOperation(false, true);
    auto const includeElementsInBoth = logicalOperation(true, true);

    if (includeElementsInAOnly && includeElementsInBoth && includeElementsInBOnly)
    {
        set_segregate(setA, setB, out, out, out);
    }
    else if (includeElementsInAOnly && includeElementsInBoth && !includeElementsInBOnly)
    {
        set_segregate(setA, setB, out, out, dead_end_iterator{});
    }
    else if (includeElementsInAOnly && !includeElementsInBoth && includeElementsInBOnly)
    {
        set_segregate(setA, setB, out, dead_end_iterator{}, out);
    }
    else if (includeElementsInAOnly && !includeElementsInBoth && !includeElementsInBOnly)
    {
        set_segregate(setA, setB, out, dead_end_iterator{}, dead_end_iterator{});
    }
    else if (!includeElementsInAOnly && includeElementsInBoth && includeElementsInBOnly)
    {
        set_segregate(setA, setB, dead_end_iterator{}, out, out);
    }
    else if (!includeElementsInAOnly && includeElementsInBoth && !includeElementsInBOnly)
    {
        set_segregate(setA, setB, dead_end_iterator{}, out, dead_end_iterator{});
    }
    else if (!includeElementsInAOnly && !includeElementsInBoth && includeElementsInBOnly)
    {
        set_segregate(setA, setB, dead_end_iterator{}, dead_end_iterator{}, out);
    }
    else if (!includeElementsInAOnly && !includeElementsInBoth && !includeElementsInBOnly)
    {
        set_segregate(setA, setB, dead_end_iterator{}, dead_end_iterator{}, dead_end_iterator{});
    }
    return out;
}

} // namespace fluent

#endif /* set_logic_operation_h */
