#include "common.hpp"
#include "sink.hpp"
#include "graphnode.hpp"
#include "graph.hpp"
#include "outputgraphnode.hpp"


NAMESPACE_BEGIN(QCE);

OutputGraphNode::OutputGraphNode(Widget *parent, Graph *parentGraph, const std::string &title) :
    GraphNode(parent, parentGraph, title)
{
    setId(fmt::format("outputGraphNode-{}", 0));

    mOutputSink = new Sink(this, mParentGraph, "Out");
    mOutputSink->setId(fmt::format("{}NodeInputSink", mId));
    mOutputSink->setIndex(mNumInputs);
    addSink(mOutputSink);

    mCameraSink = new Sink(this, mParentGraph, "Camera");
    mCameraSink->setId(fmt::format("{}CameraSink", mId));
    mNumInputs++;
    mCameraSink->setIndex(mNumInputs);
    addSink(mCameraSink);
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
