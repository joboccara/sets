# sets
The `sets` library provides an extension to the 4 algorithms on sets that the C++ STL features, which are:

* `set_union`
* `set_intersection`
* `set_difference`
* `set_symmetric_difference`

Those algorithms extract different subsets of the 2 sets they take in arguments. Here are [more details](https://www.fluentcpp.com/2017/01/09/know-your-algorithms-algos-on-sets/ "STL algorithms on sets") about those 4 STL algorithms.

By set, we mean a **sorted** range of data. For instance, `std::set`, `std::map` and a sorted `std::vector` are sets in this regard.

Although the 4 algorithms in the STL are powerful, there are things that they can't do: in particular they can't output values from both sets at the same time, or aggregate two sets together for instance.

Here are the algorithms that the `sets` library adds to extends the capacities offered by the STL (they all live in the namespace `fluent`):

## `set_segregate`
This is a central algorithm in the `sets` library,  as most of the other algorithms derive from it. `set_segregate` extracts the data from the 2 sets and segregates it into 3 parts:

* the values that are only in the first set
* the values that both in the first set and the second set
* the values that are only in the second set

Here is its interface:

```cpp
template<typename LeftRange, typename RightRange,
         typename OutputItLeft, typename OutputItBoth, typename OutputItRight>
void set_segregate(LeftRange const& leftRange, RightRange const& rightRange,
                   OutputItLeft leftOnly, OutputItBoth both, OutputItRight rightOnly)
```

(there is also an overload that accepts a custom comparator.)

All three outputs are sorted.

`OutputItBoth` accepts an iterator that can have 2 sorts of value type:

* **either the value type of the first set**: in this case `set_segregate` outputs the values of the first set that are also in the second set. This is similar to what the STL does, for instance in `set_union`.
* **or a `std::pair` of the value type of the first set and value type of the second set**: in this case `set_segregate` outputs to the pairs the values that are in both sets, with their two instances coming from each of the two sets.

The type of the output iterator passed is examined at compile-time for the algorithm to make that choice.

## `set_aggregate`

`set_aggregate` takes in two sets and agregates them. It accepts an aggregator function that it applies to equivalent elements between the two sets to aggregate them into the output.

```cpp
template<typename Range1, typename Range2, typename OutputIterator,
         typename Compare, typename Function>
OutputIterator set_aggregate(Range1 const& range1, Range2 const& range2,
                             OutputIterator output, Compare compare, Function aggregator)

```

So `set_agregate` sends to its output iterator:
* the elements that are only in the first set
* the elements that are only in the second set
* the aggregated elements that are present in both sets.

Its output is sorted.

## `set_logical_operation`

This comes from an observation that the 4 sets algorithms of the STL could be generalized and extended into one algorithm that takes a logical operation. Let's call this algorithm `set_logical_operation`, until we find a better name.
For instance, `std::set_union` would be equivalent to `set_logical_operation` with the OR logical operator, and `std::set_symmetric_difference` equivalent to the one with the XOR operator.

```cpp
template<typename Range1, typename Range2, typename OutputIterator,
         typename LogicalOperation>
OutputIterator set_logical_operation(Range1 const& range1, Range2 const& range2,
                                     OutputIterator out, LogicalOperation logicalOperation)
```

The function prototype expected by `LogicalOperation` takes 2 `bool`s in parameter and outputs 1 `bool`. For instance, the logical operation that would make the `set_logical_operation` equivalent to `std::set_difference` is:

```cpp
bool leftOnly(bool inLeft, bool inRight)
{
    return inLeft && !inRight;
}
```

The output of the algorithm is sorted.

## `set_match`

`set_match` is similar to `std::set_union`, except that where `std::set_union` only outputs the values coming from the first set that are in common with the second set, `set_match` output both the values from the first set and from the second set.

The output of `set_match` is an output iterator whose value type is a `std::pair` of the value types of the first and second set.

This is useful when making a comparison on a key within the elements compared, as values with equivalent keys could be different between the two sets.

```cpp
template<typename Range1, typename Range2, typename OutputIterator, typename Comparator>
OutputIterator set_match(Range1 const& range1, Range2 const& range2,
                         OutputIterator out, Comparator comparator)
```

The output is sorted.

## `set_share_element`

`set_share_element` takes two sets and outputs a `bool`, indicating whether their is at least one element in common (in terms of equivalence) between the two sets.

Like the other algorithms on sets, `set_share_element` operates in linear complexity. It stops whenever it finds a common element (or when it reaches the end of the sets if they have no common elements).

```cpp
template<class LeftRange, class RightRange, typename Compare>
bool set_share_element(LeftRange const& leftRange, RightRange const& rightRange, Compare comp)
```

## `disjoint`

`disjoint` takes two sorted collections and returns a `bool` equal to `true` if there is no common element between the two collections.

Complexity: sum of the sizes of the sets.
Pre-conditions: expects two sorted collections as inputs.

```cpp
template <typename Set1, typename Set2, typename Compare = std::less<>>
bool disjoint(Set1&& set1, Set2&& set2, Compare&& comp = std::less<>{})
```

## `equivalent`

`equivalent` takes two sorted collections and returns a `bool` indicating whether both collections contains only equivalent elements.
It is in the same sprit as `std::equal`, but where `std::equal` uses equality (based on `==`), `equivalent` uses [equivalence](https://www.fluentcpp.com/2017/02/16/custom-comparison-equality-equivalence-stl/ "Equality, equivalence and custom comparisons with the STL") (based on `<`) instead for correctness for operating on sorted collections.

```cpp
template <typename Set1, typename Set2, typename Compare = std::less<>>
bool equivalent(Set1&& set1, Set2&& set2, Compare&& comp = std::less<>{})
```

## `is_prefix_of`

`is_prefix_of` takes two sorted collections and returns a `bool` indicating whether the elements of the first one are contained in a contiguous manner at the beginning of the second one.
Note that this is not equivalent to using the `std::mismatch` algorithm, that uses equality (`==`). Since it operates on sorted collections, `is_prefix_of` uses [equivalence and not equality](https://www.fluentcpp.com/2017/02/16/custom-comparison-equality-equivalence-stl/ "Equality, equivalence and custom comparisons with the STL") for correctness.

Complexity: sum of the sizes of the sets.
Pre-conditions: expects two sorted collections as inputs.

```cpp
template <typename Set1, typename Set2, typename Compare = std::less<>>
bool is_prefix_of(Set1&& set1, Set2&& set2, Compare&& comp = std::less<>{})
```

## `is_before`

`is_before` takes two sorted collections and returns a `bool` indicating whether all the elements of the first collection are all smaller than the smallest element of the second one.
By convention, the empty set `is_before` any other set.

Complexity: sum of the sizes of the sets.
Pre-conditions: expects two sorted collections as inputs.

```cpp
template <typename Set1, typename Set2, typename Compare = std::less<>>
bool is_before(Set1&& set1, Set2&& set2, Compare&& comp = std::less<>{})
```

## `is_after`

`is_after` takes two sorted collections and returns a `bool` indicating whether all the elements of the second collection are all smaller than the smallest element of the first one.
By convention, any non-empty set `is_after` the empty set.

Complexity: sum of the sizes of the sets.
Pre-conditions: expects two sorted collections as inputs.

```cpp
template <typename Set1, typename Set2, typename Compare = std::less<>>
bool is_after(Set1&& set1, Set2&& set2, Compare&& comp = std::less<>{})
```

## Feedback and contributions welcome

This is an experimental library to complement the STL algorithms on sets. If you see other algorithms to add, or better names for the algorithms in the library, or any other suggestion, feel free to provide feedback or contribute!
