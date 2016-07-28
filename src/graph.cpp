#include <nanogui/layout.h>
#include "common.hpp"
#include "directpopup.hpp"
#include "clickablelabel.hpp"
#include "glshadersource.hpp"
#include "glshaderobject.hpp"
#include "graphnodelink.hpp"
#include "connector.hpp"
#include "graphnode.hpp"
#include "genericgraphnode.hpp"
#include "mergegraphnode.hpp"
#include "outputgraphnode.hpp"
#include "sink.hpp"
#include "qce.hpp"
#include "graph.hpp"


NAMESPACE_BEGIN(QCE);

Graph::Graph(Widget *parent, Qce *qce, const std::string &title) :
    Window(parent, title),
    mQce(qce),
    mPopup(nullptr),
    mActiveConnector(nullptr),
    mActiveLink(nullptr),
    mActiveNode(nullptr)
{
    mPopup = new DirectPopup(this, this);
    mPopup->setId("addPopup");
    mPopup->setSize(Eigen::Vector2i(10, 10));
    mPopup->setLayout(new nanogui::GroupLayout());
    mPopup->setVisible(false);

    nanogui::ref<ClickableLabel> addNodeButton = new ClickableLabel(
        mPopup,
        fmt::format("Add merge node")
    );
    addNodeButton->setCallback([this](const Eigen::Vector2i &p) {
        addNodeButtonEvent(p, new MergeGraphNode(this, this));
    });

    mOutputNode = new OutputGraphNode(this, this, "Output");
}

void Graph::addNodeType(GLShaderSource *shaderSource)
{
    spdlog::get("qde")->debug(
        "Adding nodeType for selection: {}",
        shaderSource->name()
    );
    nanogui::ref<ClickableLabel> addNodeButton = new ClickableLabel(
        mPopup,
        fmt::format("Add {} node", shaderSource->name())
    );
    addNodeButton->setCallback([this, shaderSource](const Eigen::Vector2i &p) {
        addNodeButtonEvent(p, shaderSource);
    });
}

void Graph::calculateOutput()
{
    std::string nodeOuptut = mOutputNode->calculateOutput();

    if (!nodeOuptut.empty()) {
        std::string output = fmt::format(
            "result = {}", mOutputNode->calculateOutput()
        );
        spdlog::get("qde")->debug("Graph {}: Calculated and set output {}", id(), output);
        mQce->setShaderOutput(output);
    }

    auto cameraSink = mOutputNode->cameraSink();
    if (cameraSink->isConnected()) {
        auto link = cameraSink->link();
        auto cameraNode = link->source()->parent();
        mQce->setShaderCamera(cameraNode->calculateOutput());
        spdlog::get("qde")->debug("Graph {}: Calculated and set camera {}", id(), cameraNode->calculateOutput());
    }
}

void Graph::nodeSelectedEvent(GraphNode *node)
{
    spdlog::get("qde")->debug("Graph: Node '{}' was selected", node->id());

    if (mActiveNode != nullptr) {
        mActiveNode->shaderObject()->hideForm();
    }

    auto shaderObject = node->shaderObject();
    if (shaderObject != nullptr) {
        if (shaderObject->hasProperties()) {
            spdlog::get("qde")->debug("Graph: Rendering properties of node '{}'", node->id());
            shaderObject->showForm();
            mActiveNode = node;
        }
    }
}

void Graph::nodeConnectedEvent(Connector *source, Connector *target)
{
    spdlog::get("qde")->debug("Graph: Source {} was connected to target {}", source->id(), target->id());

    auto shaderParameter = target->shaderParameter();

    // Set input to source shader object only if we actually have a shader set
    if (shaderParameter != nullptr) {
        target->shaderParameter()->setInput(source->parent()->shaderObject());
    }

    // Add shader source only if we actually have a shader set
    if (source->parent()->shaderObject() != nullptr) {
        mQce->addShaderToOutput(source->parent()->shaderObject());
    }
}

void Graph::connectorDraggedEvent(Connector *source, const Eigen::Vector2i &p, bool isActive)
{
    // isActive == 1 --> clicked
    // isActive == 0 --> not clicked anymore
    //
    // if isActive == 1: Link or not?
    //      if not: Create link
    //      if so:  Ignore for now
    // 
    // if isActive == 0: Dropped over
    //      if so: connect link
    //      if not: Delete link

    if (isActive) {
        if (!source->hasLink()) {
            spdlog::get("qde")->debug("Graph: Connector '{}' is active but has no link, creating", source->label());
            // The source node is not yet connected, create a new
            // connector (without valid target yet)
            auto link = new GraphNodeLink(this, source);
            link->setTargetPosition(p);
            source->setLink(link);
            mActiveLink = link;
        }
        mActiveConnector = source;
        spdlog::get("qde")->debug("Graph: Connector '{}' set as active", source->label());
    }
    else {
        if (
                // Make sure it is not the same connector
                mActiveConnector.get() != source &&
                // and also not on the same node
                mActiveConnector->parent() != source->parent()
        ) {
            // We seem to have a connection between the currently active connector
            // and the source who triggered the event.
            //
            // mActiveConnector: Source
            // Source:           Target

            auto link = new GraphNodeLink(this, source);
            link->setTargetPosition(p);
            link->setSource(mActiveConnector);
            link->setSink(source);

            mActiveConnector->setLink(link);
            source->setLink(link);

            mActiveConnector->parent()->sinkConnectedEvent(mActiveConnector);
            nodeConnectedEvent(mActiveConnector, source);
            calculateOutput();
            spdlog::get("qde")->debug("Graph: Connector '{}' was released over '{}', connection established", source->label(), mActiveConnector->label());
            mActiveConnector = nullptr;
            mActiveLink = nullptr;
            spdlog::get("qde")->debug("Graph: Connector '{}' set as inactive", source->label());
        }
        else {
            auto source = mActiveLink->source();
            source->setLink(nullptr);
            this->removeChild(mActiveLink.get());
            spdlog::get("qde")->debug("Graph: Link of connector '{}' deleted", source->label());

        }
    }
}

bool Graph::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "Graph '{}': Received mouse button event at ({},{}): {}, {}",
        mTitle, p.x(), p.y(), button, down
    );
    Window::mouseButtonEvent(p, button, down, modifiers);

    if (button == GLFW_MOUSE_BUTTON_2 && mEnabled && down) {
        int offsetX = p.x() - absolutePosition().x();
        int offsetY = p.y() - absolutePosition().y();
        Eigen::Vector2i position(offsetX, offsetY);
        mPopup->setAnchorPos(position);
        mPopup->setVisible(!mPopup->visible());
        spdlog::get("qde")->debug("Graph '{}': Mouse button 2 event handled", mTitle);

        return true;
    }
    else if (button == GLFW_MOUSE_BUTTON_1 && mEnabled) {
        /*
        if (mActiveLink.get() != nullptr && !mActiveLink->isConnected()) {
            // TODO: Check if this really is the proper way to delete an
            // object. What happens with the instance here?
            auto source = mActiveLink->source();
            source->setLink(nullptr);
            this->removeChild(mActiveLink.get());
            mActiveLink = nullptr;
            // TODO: mActiveConnector->setLink(nullptr);
            // TODO: mActiveLink = nullptr
            // TODO: mActiveConnector = nullptr;
        }
        */

        if (mPopup->visible()) {
            mPopup->setVisible(false);
        }

        return true;
    }
    else if (button == GLFW_MOUSE_BUTTON_2 && mEnabled && down) {
        int offsetX = p.x() - absolutePosition().x();
        int offsetY = p.y() - absolutePosition().y();
        Eigen::Vector2i position(offsetX, offsetY);
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

void Graph::addNodeButtonEvent(const Eigen::Vector2i &p, GLShaderSource *shaderSource)
{
    spdlog::get("qde")->debug("Graph: Add shader node button was pressed at ({}, {})", p.x(), p.y());

    nanogui::ref<GLShaderObject> shaderObject = new GLShaderObject(shaderSource, mQce);

    nanogui::ref<GenericGraphNode> node = new GenericGraphNode(this, this, shaderObject->name());
    node->setPosition(p);
    node->setEnabled(true);
    node->setVisible(true);
    node->setShaderObject(shaderObject);

    // Add parameters: Properties and/or inputs
    int index = 0;
    for (GLShaderObjectParameter param : shaderSource->parameters()) {
        GLShaderParameter *shaderParameter = shaderObject->addParameter(param);

        if (shaderParameter->parameterType() == ParameterType::INPUT) {
            nanogui::ref<Sink> input = new Sink(node, this, shaderParameter->name());
            input->setShaderParameter(shaderParameter);
            input->setIndex(index);
            index++;
        }
    }

    mQce->performLayout();

    spdlog::get("qde")->debug("Graph: Set shader node position to ({}, {})", p.x(), p.y());
    mPopup->setVisible(false);
    shaderSource->incTimesUsed();
}

void Graph::addNodeButtonEvent(const Eigen::Vector2i &p, GraphNode *graphNode)
{
    spdlog::get("qde")->debug("Graph: Add graph node button was pressed at ({}, {})", p.x(), p.y());

    // TODO: Implement this
    graphNode->setPosition(p);
    graphNode->setEnabled(true);
    graphNode->setVisible(true);

    mQce->performLayout();

    spdlog::get("qde")->debug("Graph: Set graph node position to ({}, {})", p.x(), p.y());
    mPopup->setVisible(false);
}

NAMESPACE_END(QCE);
