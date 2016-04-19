#include "common.hpp"
#include "sink.hpp"
#include "graphnode.hpp"
#include "graph.hpp"
#include "outputgraphnode.hpp"


NAMESPACE_BEGIN(QCE);

OutputGraphNode::OutputGraphNode(Widget *parent, const std::string &title) :
    GraphNode(parent, title)
{
    setId(fmt::format("outputGraphNode-{}", 0));
    Graph *parentGraph = dynamic_cast<Graph *>(parent);
    Sink *sink = new Sink(this, parentGraph, "Out");
    sink->setId("outputGraphNodeInputSink");
    addSink(sink);
}

void OutputGraphNode::performLayout(NVGcontext *ctx)
{
    GraphNode::performLayout(ctx);
    mPos = Eigen::Vector2i(
        mParent->size().x() - mSize.x() - 10,
        mParent->size().y() / 2 - mSize.y() / 2
    );
}

NAMESPACE_END(QCE);