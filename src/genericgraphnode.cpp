#include "main.hpp"

GenericGraphNode::GenericGraphNode(Widget *parent, const std::string &title) :
    GraphNode(parent, title)
{
    {
        Graph *parentGraph = dynamic_cast<Graph *>(parent);
        OutputSink *sink = new OutputSink(this, parentGraph, "Output");
        sink->setId(fmt::format("{}GraphNodeOutputSink", title));
        addOutputSink(sink);
    }
}

void GenericGraphNode::performLayout(NVGcontext *ctx)
{
    GraphNode::performLayout(ctx);
    mPos = Vector2i(
        mParent->size().x() - mSize.x() - 10,
        mParent->size().y() / 2 - mSize.y() / 2
    );
}

std::string GenericGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);
    return mShaderObject->call();
}
