#include "main.hpp"

Graph::Graph(Widget *parent, const std::string &title) :
    Window(parent, title),
    mSelectedNodeIndex(-1)
{
    {
        mPopup = new DirectPopup(this, this);
        mPopup->setSize(Vector2i(10, 10));
        mPopup->setLayout(new GroupLayout());
        mPopup->setVisible(false);
    }

    {
        ClickableLabel *addNodeButton  = new ClickableLabel(mPopup, "Add dummy node");
        addNodeButton->setCallback([this](const Vector2i &p) {
            this->addNodeButtonEvent(p);
        });
    }

    OutputGraphNode *output = new OutputGraphNode(this, "Output");
    InputSink *sink = new InputSink(output, "Output");
    output->addInputSink(sink);
}

bool Graph::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    Window::mouseButtonEvent(p, button, down, modifiers);

    if (button == GLFW_MOUSE_BUTTON_2 && mEnabled && down) {
        int offsetX = p.x() - absolutePosition().x();
        int offsetY = p.y() - absolutePosition().y();
        Vector2i position(offsetX, offsetY);
        mPopup->setAnchorPos(position);
        mPopup->setVisible(!mPopup->visible());

        return true;
    }

    return false;
}

void Graph::performLayout(NVGcontext *ctx)
{
    spdlog::get("qde")->debug("Performing layout on graph at ({}, {})", mPos.x(), mPos.y());
    Window::performLayout(ctx);
}

void Graph::addNodeButtonEvent(const Vector2i &p) {
    spdlog::get("qde")->debug("Add node button was pressed on '{}'", mTitle);
    spdlog::get("qde")->debug("Add node button was pressed at ({}, {})", p.x(), p.y());

    GraphNode *node = new GraphNode(this, "Dummy node");
    node->setPosition(p);
    OutputSink *sink = new OutputSink(node, "Output");
    node->addOutputSink(sink);

    Qce *qce = dynamic_cast<Qce *>(mParent);
    qce->setIntensity(0.8f);
    qce->bindShader();

    mPopup->setVisible(false);
}
