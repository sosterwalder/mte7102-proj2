#include "common.hpp"
#include "source.hpp"
#include "sink.hpp"
#include "graph.hpp"
#include "mergegraphnode.hpp"


NAMESPACE_BEGIN(QCE);

MergeGraphNode::MergeGraphNode(Widget *parent, Graph *parentGraph, const std::string &title) :
    GraphNode(parent, parentGraph, title)
{
    Source *source = new Source(this, mParentGraph, "Output");
    source->setId(fmt::format("{}GraphNodeSource", title));
    addSource(source);
    
    mNumInputs = 1;
    Sink *input = new Sink(this, mParentGraph, fmt::format("Input {}", mNumInputs));
    input->setId(fmt::format("{}GraphNodeInput{}", title, mNumInputs));
    addSink(input);
}

bool MergeGraphNode::sinkConnectedEvent(Connector *sink)
{
    GraphNode::sinkConnectedEvent(sink);

    mNumInputs++;
    Sink *input = new Sink(this, mParentGraph, fmt::format("Input {}", mNumInputs));
    input->setId(fmt::format("{}GraphNodeInput{}", mTitle, mNumInputs));
    input->setIndex(mNumInputs);
    addSink(input);

    return true;
}

std::string MergeGraphNode::calculateOutput()
{
    spdlog::get("qde")->debug("Node '{}': Calculating output", mId);
    return GraphNode::calculateOutput();

    /*

    fmt::MemoryWriter out;

    for (auto iter = mInputs.begin(); iter != mInputs.end(); ++iter) {
        auto sink = *iter;

        if (sink->isConnected()) {
            auto link = sink->link();
            auto source =

        }
    }

    return out.str();
     */
}

NAMESPACE_END(QCE);
