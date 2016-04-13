#include "main.hpp"

Graph::Graph(Widget *parent, Qce *qce, const std::string &title) :
    Window(parent, title),
    mQce(qce),
    mActiveConnector(nullptr),
    mSelectedNodeIndex(-1)
{
    mPopup = new DirectPopup(this, this);
    mPopup->setId("addPopup");
    mPopup->setSize(Vector2i(10, 10));
    mPopup->setLayout(new GroupLayout());
    mPopup->setVisible(false);

    mOutputNode = new OutputGraphNode(this, "Output");
}

void Graph::addNodeType(GLShaderObject *shaderObject)
{
    spdlog::get("qde")->debug(
        "Adding nodeType for selection: {}",
        shaderObject->name()
    );
    ClickableLabel *addNodeButton  = new ClickableLabel(
        mPopup,
        fmt::format("Add {} node", shaderObject->name())
    );
    addNodeButton->setCallback([this, shaderObject](const Vector2i &p) {
        this->addNodeButtonEvent(p, shaderObject);
    });
}

void Graph::calculateOutput()
{
    std::string output = fmt::format(
        "result = {}", mOutputNode->calculateOutput()
    );
    spdlog::get("qde")->debug("Graph {}: Calculated and set output {}", id(), output);
    mQce->setShaderOutput(output);
}

void Graph::drawContents()
{
    // mOutputNode->drawContents();
    mQce->bindShader();
}

bool Graph::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    if (Window::mouseButtonEvent(p, button, down, modifiers)) {
        return true;
    }

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

void Graph::addNodeButtonEvent(const Vector2i &p, GLShaderObject *shaderObject)
{
    spdlog::get("qde")->debug("Graph: Add node button was pressed at ({}, {})", p.x(), p.y());

    GenericGraphNode *node = new GenericGraphNode(this, shaderObject->id());
    shaderObject->incTimesUsed();
    node->setId(shaderObject->id());
    node->setPosition(p);
    spdlog::get("qde")->debug("Graph: Set node position to ({}, {})", p.x(), p.y());
    node->setEnabled(true);
    node->setVisible(true);
    node->setShaderObject(shaderObject);

    mQce->addShaderToOutput(shaderObject);

    mPopup->setVisible(false);
    mQce->performLayout();
}
