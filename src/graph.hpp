#pragma once

#include <nanogui/window.h>

NAMESPACE_BEGIN(QCE)

class DirectPopup;
class GLShaderSource;
class GLShaderObject;
class Connector;
class GraphNodeLink;
class GraphNode;
class Qce;


class Graph : public nanogui::Window
{
public:
    Graph(nanogui::Widget *parent, Qce *qce, const std::string &title = "Untitled graph");
    void setActiveConnector(Connector *connector) { mActiveConnector = connector; }
    Connector *activeConnector() { return mActiveConnector; }
    const Connector *activeConnector() const { return mActiveConnector.get(); }
    void addNodeType(GLShaderSource *shaderSource);
    void calculateOutput();
    void nodeSelectedEvent(GraphNode *node);
    void nodeConnectedEvent(Connector *source, Connector *target);
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual void performLayout(NVGcontext *ctx);

protected:
    nanogui::ref<Qce> mQce;
    nanogui::ref<DirectPopup> mPopup;
    nanogui::ref<Connector> mActiveConnector;
    nanogui::ref<GraphNode> mOutputNode;
    nanogui::ref<GraphNode> mActiveNode;
    std::vector<GraphNode> mNodes;
    std::vector<GraphNodeLink> mNodeLinks;

    virtual void addNodeButtonEvent(const Eigen::Vector2i &p, GLShaderSource *shaderSource);
};

NAMESPACE_END(QCE);