#include <nanogui/theme.h>
#include <nanogui/widget.h>
#include "common.hpp"
#include "connector.hpp"
#include "graphnode.hpp"
#include "graph.hpp"
#include "source.hpp"


NAMESPACE_BEGIN(QCE);

Eigen::Vector2i Source::relativePosition()
{
    int offset = mSize.y() + 1;
    
    return Eigen::Vector2i(
        mParent->size().x() - mSize.x(),
        mParent->theme()->mWindowHeaderHeight + offset * mIndex
    );
}

NAMESPACE_END(QCE);