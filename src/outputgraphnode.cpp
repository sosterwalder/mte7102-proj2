#include "main.hpp"

OutputGraphNode::OutputGraphNode(Widget *parent, const std::string &title) :
    GraphNode(parent, title)
{
}

void OutputGraphNode::performLayout(NVGcontext *ctx)
{
    Window::performLayout(ctx);
    mPos = Vector2i(
        mParent->size().x() - mSize.x() - 10,
        mParent->size().y() / 2 - mSize.y() / 2
    );
}
