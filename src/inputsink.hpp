#pragma once

#include "main.hpp"

class InputSink : public Sink
{
public:
    InputSink(Widget *parent, const std::string &label);
    virtual void draw(NVGcontext* ctx);

protected:
    virtual void refreshRelativePlacement();
};
