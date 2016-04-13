#pragma once

#include "main.hpp"

class GenericGraphNode : public GraphNode
{
public:
    GenericGraphNode(Widget *parent, const std::string &title = "Undefined");
    // virtual void performLayout(NVGcontext *ctx);
    virtual std::string calculateOutput();
};

