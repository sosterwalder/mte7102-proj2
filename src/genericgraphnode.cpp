#include "common.hpp"
#include "glshaderobject.hpp"
#include "source.hpp"
#include "graph.hpp"
#include "genericgraphnode.hpp"


NAMESPACE_BEGIN(QCE);

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

void GenericGraphNode::performLayout(NVGcontext *ctx)
{
    GraphNode::performLayout(ctx);
}

std::string GenericGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);
    return mShaderObject->call();
}

NAMESPACE_END(QCE);