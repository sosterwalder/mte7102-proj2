#pragma once

#include "main.hpp"

class OutputSink : public Sink
{
public:
    OutputSink(Widget *parent, Graph *parentGraph, const std::string &label);

protected:
    virtual Vector2i relativePosition();
};
