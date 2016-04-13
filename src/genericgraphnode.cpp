#include "main.hpp"

GenericGraphNode::GenericGraphNode(Widget *parent, const std::string &title) :
    GraphNode(parent, title)
{
    {
        Graph *parentGraph = dynamic_cast<Graph *>(parent);
        Source *source = new Source(this, parentGraph, "Output");
        source->setId(fmt::format("{}GraphNodeSource", title));
        addSource(source);
    }
}

/*void GenericGraphNode::performLayout(NVGcontext *ctx)
{
    GraphNode::performLayout(ctx);
    mPos = Vector2i(
        mParent->size().x() - mSize.x() - 10,
        mParent->size().y() / 2 - mSize.y() / 2
    );
}
*/

std::string GenericGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);
    return mShaderObject->call();
}
