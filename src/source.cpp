#include "main.hpp"

Source::Source(Widget *parent, Graph *parentGraph, const std::string &label) :
    Connector(parent, parentGraph, label)
{
}

Vector2i Source::relativePosition()
{
    return Vector2i(
        mParent->size().x() - 10,
        (mParent->size().y() + mTheme->mWindowHeaderHeight) / 2 - mSize.y() / 2
    );
}
