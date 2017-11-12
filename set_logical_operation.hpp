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
#include "output_transformer.hpp"
#include "set_seggregate.hpp"

namespace fluent
{
namespace details
{
    
template<typename Range1, typename Range2, typename OutputIterator, typename LogicalOperation, typename OutputIteratorLeft, typename OutputIteratorBoth>
void set_logical_operation_impl(Range1 const& range1, Range2 const& range2, OutputIterator out, LogicalOperation logicalOperation, OutputIteratorLeft outLeft, OutputIteratorBoth outBoth)
{
    if (logicalOperation(false, true)) // in right and not in left
    {
        set_seggregate(range1, range2, outLeft, outBoth, out);
    }
    else
    {
        set_seggregate(range1, range2, outLeft, outBoth, dead_end_iterator());
    }
}

template<typename Range1, typename Range2, typename OutputIterator, typename LogicalOperation, typename OutputIteratorLeft>
void set_logical_operation_impl(Range1 const& range1, Range2 const& range2, OutputIterator out, LogicalOperation logicalOperation, OutputIteratorLeft outLeft)
{
    if (logicalOperation(true, true)) // both in left and in right
    {
        set_logical_operation_impl(range1, range2, out, logicalOperation, outLeft, out);
    }
    else
    {
        set_logical_operation_impl(range1, range2, out, logicalOperation, outLeft, dead_end_iterator());
    }
}
} // namespace details
    
template<typename Range1, typename Range2, typename OutputIterator, typename LogicalOperation>
void set_logical_operation(Range1 const& range1, Range2 const& range2, OutputIterator out, LogicalOperation logicalOperation)
{
    if (logicalOperation(true, false)) // in left and not in right
    {
        details::set_logical_operation_impl(range1, range2, out, logicalOperation, out);
    }
    else
    {
        details::set_logical_operation_impl(range1, range2, out, logicalOperation, dead_end_iterator());
    }
}

} // namespace fluent

#endif /* set_logic_operation_h */
