#pragma once

#include "main.hpp"

class GraphNodeLink : public Widget
{
public:
    GraphNodeLink(Widget *parent);
    GraphNodeLink(Widget *parent, int inputIndex, int inputSlot, int outputIndex, int outputSlot);

protected:
    int mInputIndex;
    int mInputSlot;
    int mOutputIndex;
    int mOutputSlot;
};
