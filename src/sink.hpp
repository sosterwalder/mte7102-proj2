#pragma once

#include "connector.hpp"

NAMESPACE_BEGIN(QCE);

class Graph;


class Sink : public Connector
{
public:
    Sink(GraphNode *parent, Graph *parentGraph, const std::string &label) :
        Connector(parent, parentGraph, label) {}

protected:
    virtual Eigen::Vector2i relativePosition();
};

NAMESPACE_END(QCE);