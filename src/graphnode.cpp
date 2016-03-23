#include "main.hpp"

GraphNode::GraphNode(Widget *parent, const std::string &title) :
    Window(parent, title),
    mAnchorPos(Vector2i::Zero()),
    mAnchorHeight(30)
{
    mSize = Vector2i(100, 80);
}

Vector2i GraphNode::getInputSlotPosition(const int slotNumber) const
{
    float offset = ((float)slotNumber + 1.0f) / ((float)mNumInputs + 1);

    return this->getSlotPosition(offset);
}

Vector2i GraphNode::getOutputSlotPosition(const int slotNumber) const
{
    float offset = ((float)slotNumber + 1.0f) / ((float)mNumOutputs + 1);

    return this->getSlotPosition(offset);
}

void GraphNode::draw(NVGcontext* ctx)
{
    refreshRelativePlacement();
    Window::draw(ctx);
}

void GraphNode::addInputSink(InputSink* sink)
{
    mInputs.push_back(sink);
    sink->incRef();
}

void GraphNode::removeInputSink(const int index)
{
    InputSink *sink = mInputs[index];
    mInputs.erase(mInputs.begin() + index);
    sink->decRef();
}

void GraphNode::removeInputSink(const InputSink *sink)
{
    mInputs.erase(std::remove(
        mInputs.begin(),
        mInputs.end(),
        sink
    ), mInputs.end());
    sink->decRef();
}

void GraphNode::addOutputSink(OutputSink *sink)
{
    mOutputs.push_back(sink);
    sink->incRef();
}

void GraphNode::removeOutputSink(const int index)
{
    OutputSink *sink = mOutputs[index];
    mOutputs.erase(mOutputs.begin() + index);
    sink->decRef();
}

void GraphNode::removeOutputSink(const OutputSink *sink)
{
    mOutputs.erase(std::remove(
        mOutputs.begin(),
        mOutputs.end(),
        sink
    ), mOutputs.end());
    sink->decRef();
}

void GraphNode::refreshRelativePlacement()
{
    // Will be overridden in child classes
}

Vector2i GraphNode::getSlotPosition(const float offset) const
{
    return Vector2i(
        mPos.x(),
        mPos.y() + mSize.y() * offset
    );
}
