#pragma once

#include "graphnode.hpp"


NAMESPACE_BEGIN(QCE);

class GenericGraphNode : public GraphNode
{
public:
    GenericGraphNode(Widget *parent, Graph* parentGraph, const std::string &title = "Undefined");
    virtual void performLayout(NVGcontext *ctx);
    virtual std::string calculateOutput();
};

NAMESPACE_END(QCE);