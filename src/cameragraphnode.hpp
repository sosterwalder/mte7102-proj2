#pragma once

#include "graphnode.hpp"

NAMESPACE_BEGIN(QCE);

class CameraGraphNode : public GraphNode
{
public:
    CameraGraphNode(Widget *parent, Graph *parentGraph, const std::string &title = "Camera") :
        GraphNode(parent, parentGraph, title) {}
    virtual std::string calculateOutput();
};

NAMESPACE_END(QCE);

