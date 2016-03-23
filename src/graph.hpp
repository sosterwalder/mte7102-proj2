#pragma once

#include "main.hpp"

using namespace nanogui;

class Graph : public Window
{
public:
    Graph(Widget *parent, const std::string &title = "Untitled graph");
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual void performLayout(NVGcontext *ctx);

protected:
    DirectPopup *mPopup;
    std::vector<GraphNode> mNodes;
    std::vector<GraphNodeLink> mNodeLinks;
    int mSelectedNodeIndex;

    virtual void addNodeButtonEvent(const Vector2i &p);
};
