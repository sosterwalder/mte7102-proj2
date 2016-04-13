#include "main.hpp"

Sink::Sink(Widget *parent, Graph *parentGraph, const std::string &label) :
    Connector(parent, parentGraph, label)
{
}

Vector2i Sink::relativePosition()
{
    return Vector2i(
        10,
        (mParent->size().y() + mTheme->mWindowHeaderHeight) / 2 - mSize.y() / 2
    );
}
