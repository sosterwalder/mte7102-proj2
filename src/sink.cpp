#include "main.hpp"

Sink::Sink(Widget *parent, const std::string &label) :
    Widget(parent),
    mLabel(label),
    mTextColor(Color(0, 0)),
    mConnected(false)
{
}

void Sink::refreshRelativePlacement() {
    // Will be overriden in child classes
}
