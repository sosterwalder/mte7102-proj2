#include <nanogui/theme.h>
#include <nanogui/widget.h>
#include "common.hpp"
#include "connector.hpp"
#include "graphnode.hpp"
#include "graph.hpp"
#include "sink.hpp"


NAMESPACE_BEGIN(QCE);

Eigen::Vector2i Sink::relativePosition()
{
    int offset = mSize.y() + 1;
    
    // Offset
    // 1: 0 * offset          1 - num + num - 1 = 0
    // 2: 1 * offset          2/x=1
    // 3: 2 * offset          3/x=2
    // 4: 3 * offset          4/x=3
    // Inputs: 4
    
    return Eigen::Vector2i(
        5,
        mParent->theme()->mWindowHeaderHeight + offset * mIndex
    );
}

NAMESPACE_END(QCE);