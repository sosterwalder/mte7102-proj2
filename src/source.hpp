#pragma once

#include "main.hpp"

class Source : public Connector
{
public:
    Source(Widget *parent, Graph *parentGraph, const std::string &label);

protected:
    virtual Vector2i relativePosition();
};
