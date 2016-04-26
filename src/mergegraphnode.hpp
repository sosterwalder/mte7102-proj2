#pragma once

#include "graphnode.hpp"

NAMESPACE_BEGIN(QCE);

class MergeGraphNode : public GraphNode
{
public:
    MergeGraphNode(Widget *parent, Graph *parentGraph, const std::string &title = "Merge");
    virtual bool sinkConnectedEvent(Connector *sink);
    virtual std::string calculateOutput();
};

NAMESPACE_END(QCE);