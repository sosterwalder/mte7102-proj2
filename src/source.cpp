#include <nanogui/theme.h>
#include <nanogui/widget.h>
#include "common.hpp"
#include "connector.hpp"
#include "graph.hpp"
#include "source.hpp"


NAMESPACE_BEGIN(QCE);

Source::Source(Widget *parent, Graph *parentGraph, const std::string &label) :
    Connector(parent, parentGraph, label)
{
}

Eigen::Vector2i Source::relativePosition()
{
    return Eigen::Vector2i(
        mParent->size().x() - mSize.x(),
        (mParent->size().y() + mTheme->mWindowHeaderHeight) / 2 - mSize.y() / 2
    );
}

NAMESPACE_END(QCE);