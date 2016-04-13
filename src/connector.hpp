#pragma once

#include "main.hpp"

class Graph;
class GraphNodeLink;

class Connector : public Widget
{
public:
    Connector(Widget *parent, Graph *parentGraph, const std::string &label);
    const std::string &label() const { return mLabel; }
    GraphNodeLink *link() { return mLink; }
    const GraphNodeLink *link() const { return mLink.get(); }
    virtual void draw(NVGcontext* ctx);
    virtual Vector2i preferredSize(NVGcontext *ctx) const;
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);
    virtual bool mouseEnterEvent(const Vector2i &p, bool enter);
    virtual Vector2i relativePosition() = 0;

protected:
    ref<Graph> mParentGraph;
    ref<GraphNodeLink> mLink;
    std::string mLabel;
    Color mTextColor;
    bool mDrag;
    Vector2i mRelativePosition;

    // Internal helper function to maintain nested window position values
    void refreshRelativePlacement();
    bool isConnected() { return mLink != nullptr; }
};
