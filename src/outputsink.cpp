#include "main.hpp"

OutputSink::OutputSink(Widget *parent, Graph *parentGraph, const std::string &label) :
    Sink(parent, parentGraph, label)
{
}

Vector2i OutputSink::relativePosition()
{
    return Vector2i(
        mParent->size().x() - 10,
        (mParent->size().y() + mTheme->mWindowHeaderHeight) / 2 - mSize.y() / 2
    );
}
