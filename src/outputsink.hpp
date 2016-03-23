#pragma once

#include "main.hpp"

class OutputSink : public Sink
{
public:
    OutputSink(Widget *parent, const std::string &label);
    virtual void draw(NVGcontext* ctx);

protected:
    virtual void refreshRelativePlacement();
};
