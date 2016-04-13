#pragma once

#include "main.hpp"

class Sink : public Connector
{
public:
    Sink(Widget *parent, Graph *parentGraph, const std::string &label);

protected:
    virtual Vector2i relativePosition();
};
