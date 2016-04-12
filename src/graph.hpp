#pragma once

#include "main.hpp"

using namespace nanogui;

class Qce;

class Graph : public Window
{
public:
    Graph(Widget *parent, Qce *qce, const std::string &title = "Untitled graph");
    void setActiveSink(Sink *sink) { mActiveSink = sink; }
    Sink *activeSink() { return mActiveSink; }
    const Sink *activeSink() const { return mActiveSink.get(); }
    void addNodeType(GLShaderObject *shaderObject);
    void calculateOutput();
    virtual void drawContents();
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual void performLayout(NVGcontext *ctx);

protected:
    ref<Qce> mQce;
    DirectPopup *mPopup;
    ref<Sink> mActiveSink;
    ref<GraphNode> mOutputNode;
    std::vector<GraphNode> mNodes;
    std::vector<GraphNodeLink> mNodeLinks;
    int mSelectedNodeIndex;

    virtual void addNodeButtonEvent(const Vector2i &p, GLShaderObject *shaderObject);
};
