#include "main.hpp"

GraphNode::GraphNode(Widget *parent, const std::string &title) :
    Window(parent, title),
    mAnchorPos(Vector2i::Zero()),
    mAnchorHeight(30),
    mShaderObject(nullptr)
{
    mSize = Vector2i(100, 80);

    mPopup = new DirectPopup(this, this);
    mPopup->setId("graphNodePopup");
    mPopup->setSize(Vector2i(10, 10));
    mPopup->setLayout(new GroupLayout());
    mPopup->setVisible(false);

    ClickableLabel *removeButton  = new ClickableLabel(mPopup, "Remove");
    removeButton->setId("removeGraphNodeButton");
    removeButton->setCallback([this](const Vector2i &p) {
        spdlog::get("qde")->debug(
            "GraphNode '{}': Removing",
            mTitle
        );
        mShaderObject->decTimesUsed();
        // TODO: Check if this really is the proper way to delete an
        // object. What happens with the instance here?
        Graph *parentGraph = dynamic_cast<Graph *>(mParent);
        parentGraph->removeChild(this);
        mPopup->setVisible(false);
    });
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

bool GraphNode::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    Window::mouseButtonEvent(p, button, down, modifiers);

    if (button == GLFW_MOUSE_BUTTON_2 && mEnabled && down) {
        int offsetX = p.x() - mPos.x();
        int offsetY = p.y() - mPos.y();
        Vector2i position(offsetX, offsetY);
        mPopup->setAnchorPos(position);
        mPopup->setVisible(!mPopup->visible());

        return true;
    }

    return false;
}

// May be overridden by child classes.
std::string GraphNode::calculateOutput()
{
    fmt::MemoryWriter output;

    for (auto input : mInputs) {
        if (input->link()) {
            GraphNodeLink *link = input->link();
            // spdlog::get("qde")->debug("Node '{}': Input has link: '{}'", mId, link->id());

            if (link->source()) {
                GraphNode *node = dynamic_cast<GraphNode *>(link->source()->parent());
                output << node->calculateOutput();
                spdlog::get("qde")->debug("Node '{}': Link has source: '{}'", mId, node->id());
            }
        }
    }

    return output.str();
}

Vector2i GraphNode::getSlotPosition(const float offset) const
{
    return Vector2i(
        mPos.x(),
        mPos.y() + mSize.y() * offset
    );
}
