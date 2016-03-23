#pragma once

#include "main.hpp"

class OutputGraphNode : public GraphNode
{
public:
    OutputGraphNode(Widget *parent, const std::string &title = "Undefined");
    virtual void performLayout(NVGcontext *ctx);
};
