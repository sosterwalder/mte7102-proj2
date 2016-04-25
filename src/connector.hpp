#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(QCE);

class Graph;
class GraphNode;
class GraphNodeLink;
class GLShaderParameter;


class Connector : public nanogui::Widget
{
public:
    Connector(GraphNode *parent, Graph *parentGraph, const std::string &label);
    GraphNode *parent() { return mParent; }
    const GraphNode *parent() const { return mParent.get(); }
    const std::string &label() const { return mLabel; }
    GraphNodeLink *link() { return mLink; }
    const GraphNodeLink *link() const { return mLink.get(); }
    void setIndex(const int &index) { mIndex = index; }
    GLShaderParameter *shaderParameter() { return mShaderParameter; }
    const GLShaderParameter *shaderParameter() const { return mShaderParameter.get(); }
    void setShaderParameter(GLShaderParameter *shaderParameter) { mShaderParameter = shaderParameter; }
    virtual void draw(NVGcontext* ctx);
    virtual Eigen::Vector2i preferredSize(NVGcontext *ctx) const;
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual bool mouseDragEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers);
    virtual bool mouseEnterEvent(const Eigen::Vector2i &p, bool enter);
    virtual Eigen::Vector2i relativePosition() = 0;

protected:
    nanogui::ref<GraphNode> mParent;
    nanogui::ref<Graph> mParentGraph;
    nanogui::ref<GraphNodeLink> mLink;
    nanogui::ref<GLShaderParameter> mShaderParameter;
    std::string mLabel;
    nanogui::Color mTextColor;
    bool mDrag;
    Eigen::Vector2i mRelativePosition;
    int mIndex;

    // Internal helper function to maintain nested window position values
    void refreshRelativePlacement();
    bool isConnected() { return mLink != nullptr; }
};

NAMESPACE_END(QCE);