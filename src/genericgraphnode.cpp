#include "common.hpp"
#include "glshaderobject.hpp"
#include "source.hpp"
#include "graph.hpp"
#include "genericgraphnode.hpp"


NAMESPACE_BEGIN(QCE);

GenericGraphNode::GenericGraphNode(Widget *parent, Graph *parentGraph, const std::string &title) :
    GraphNode(parent, parentGraph, title)
{
    Source *source = new Source(this, mParentGraph, "Output");
    source->setId(fmt::format("{}GraphNodeSource", title));
    addSource(source);
}

void GenericGraphNode::performLayout(NVGcontext *ctx)
{
    GraphNode::performLayout(ctx);
}

std::string GenericGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);

    fmt::MemoryWriter out;
    out << mShaderObject->call();
    out << ";";

    return out.str();
}

NAMESPACE_END(QCE);
