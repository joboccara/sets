#ifndef METAPROG_HPP
#define METAPROG_HPP

#include <iterator>
#include <type_traits>
#include <utility>

// underlying type

template<typename Iterator>
struct iterator_underlying_type_impl
{
    using type = typename std::remove_reference<decltype(*std::declval<Iterator>())>::type;
};

template<typename Container>
struct iterator_underlying_type_impl<std::back_insert_iterator<Container>>
{
    using type = typename Container::value_type;
};

template<typename Container>
struct iterator_underlying_type_impl<std::insert_iterator<Container>>
{
    using type = typename Container::value_type;
};

template<typename Iterator>
using iterator_underlying_type = typename iterator_underlying_type_impl<Iterator>::type;

template<typename Range>
using range_underlying_type = iterator_underlying_type<decltype(std::declval<Range>().begin())>;

// is_pair

template<typename T>
struct is_pair_impl
{
    static const bool value = false;
};

template<typename T1, typename T2>
struct is_pair_impl<std::pair<T1,T2>>
{
    static const bool value = true;
};

template<typename T>
struct is_pair :
is_pair_impl < typename std::remove_const< typename std::remove_reference< T >::type >::type >
{};


// remove const even inside pair (necessary to operate on maps iterators)

template<typename T>
struct remove_all_consts_impl
{
    using type = typename std::remove_const<T>::type;
};

template<typename T1, typename T2>
struct remove_all_consts_impl<std::pair<T1, T2>>
{
    using type = std::pair<typename remove_all_consts_impl<T1>::type, typename remove_all_consts_impl<T2>::type>;
};

template<typename T1, typename T2>
struct remove_all_consts_impl<const std::pair<T1, T2>>
{
    using type = std::pair<typename remove_all_consts_impl<T1>::type, typename remove_all_consts_impl<T2>::type>;
};

template<typename T>
using remove_all_consts = typename remove_all_consts_impl<T>::type;

// does both contain versions from left AND right

template<bool isBothOutputAPair, typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRightImpl
{
    static const bool value = false;
};

template<typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRightImpl<true, BothOutputIterator, LeftRange, RightRange>
{
    static const bool value = 
        std::is_same
        <
            remove_all_consts < typename iterator_underlying_type < BothOutputIterator >::first_type >,
            remove_all_consts < range_underlying_type < LeftRange > >
        >::value
        &&
        std::is_same
        <
            remove_all_consts < typename iterator_underlying_type < BothOutputIterator >::second_type >,
            remove_all_consts < range_underlying_type < RightRange > >
        >::value;
};

template<typename BothOutputIterator, typename LeftRange, typename RightRange>
struct BothContainsLeftAndRight :
    BothContainsLeftAndRightImpl < is_pair < iterator_underlying_type < BothOutputIterator > >::value, BothOutputIterator, LeftRange, RightRange>
{};

#endif

