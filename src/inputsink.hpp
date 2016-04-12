#pragma once

#include "main.hpp"

class InputSink : public Sink
{
public:
    InputSink(Widget *parent, Graph *parentGraph, const std::string &label);

protected:
    virtual Vector2i relativePosition();
};
