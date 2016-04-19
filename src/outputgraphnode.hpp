#pragma once


NAMESPACE_BEGIN(QCE);

class OutputGraphNode : public GraphNode
{
public:
    OutputGraphNode(Widget *parent, const std::string &title = "Undefined");
    virtual void performLayout(NVGcontext *ctx);
};

NAMESPACE_END(QCE);