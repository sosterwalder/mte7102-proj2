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

void GraphNode::addSink(Sink* sink)
{
    mInputs.push_back(sink);
    sink->incRef();
}

void GraphNode::removeSink(const int index)
{
    Sink *sink = mInputs[index];
    mInputs.erase(mInputs.begin() + index);
    sink->decRef();
}

void GraphNode::removeSink(const Sink *sink)
{
    mInputs.erase(std::remove(
        mInputs.begin(),
        mInputs.end(),
        sink
    ), mInputs.end());
    sink->decRef();
}

void GraphNode::addSource(Source *source)
{
    mOutputs.push_back(source);
    source->incRef();
}

void GraphNode::removeSource(const int index)
{
    Source *source = mOutputs[index];
    mOutputs.erase(mOutputs.begin() + index);
    source->decRef();
}

void GraphNode::removeSource(const Source *source)
{
    mOutputs.erase(std::remove(
        mOutputs.begin(),
        mOutputs.end(),
        source
    ), mOutputs.end());
    source->decRef();
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
