#include "main.hpp"

InputSink::InputSink(Widget *parent, Graph *parentGraph, const std::string &label) :
    Sink(parent, parentGraph, label)
{
}

Vector2i InputSink::relativePosition()
{
    return Vector2i(
        10,
        (mParent->size().y() + mTheme->mWindowHeaderHeight) / 2 - mSize.y() / 2
    );
}
