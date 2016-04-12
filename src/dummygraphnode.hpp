#pragma once

#include "main.hpp"

class SphereGraphNode : public GraphNode
{
public:
    SphereGraphNode(Widget *parent, const std::string &title = "Undefined");
    virtual void performLayout(NVGcontext *ctx);
    virtual const std::string &calculateOutput() const;
};

