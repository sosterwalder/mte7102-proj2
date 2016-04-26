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

bool Graph::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    if (Window::mouseButtonEvent(p, button, down, modifiers)) {
        return true;
    }

    if (button == GLFW_MOUSE_BUTTON_2 && mEnabled && down) {
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
