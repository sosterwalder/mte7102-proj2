#include "main.hpp"

GraphNodeLink::GraphNodeLink(Widget *parent) :
    Widget(parent)
{
}

GraphNodeLink::GraphNodeLink(Widget *parent, int inputIndex, int inputSlot, int outputIndex, int outputSlot) :
    Widget(parent),
    mInputIndex(inputIndex),
    mInputSlot(inputSlot),
    mOutputIndex(outputIndex),
    mOutputSlot(outputSlot)
{
}
