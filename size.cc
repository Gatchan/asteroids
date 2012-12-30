#include "size.h"

Size::Size()
: width(-1.0f)
, height(-1.0f)
{
}

Size::Size(const float width, const float height)
: width(width)
, height(height)
{
}

Size::Size(const Size& other)
: width(other.width)
, height(other.height)
{
}
