#ifndef SET_SHARE_ELEMENT_HPP
#define SET_SHARE_ELEMENT_HPP

template<class LeftRange, class RightRange, typename Compare>
bool set_share_element(LeftRange const& leftRange, RightRange const& rightRange, Compare comp)
{
    auto itLeft = leftRange.begin();
    auto itRight = rightRange.begin();
    while (itLeft != leftRange.end() && itRight != rightRange.end())
    {
        if (comp(*itLeft, *itRight))
        {
            ++itLeft;
        }
        else if (comp(*itRight, *itLeft))
        {
            ++itRight;
        }
        else
        {
            return true;
        }
    }
    return false;
}

template<class LeftRange, class RightRange>
bool set_share_element(LeftRange const& leftRange, RightRange const& rightRange)
{
    return set_share_element(leftRange, rightRange, std::less<typename LeftRange::value_type>{});
}

#endif /* SET_SHARE_ELEMENT_HPP */
