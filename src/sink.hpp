#pragma once

#include "main.hpp"

class Sink : public Widget
{
public:
    Sink(Widget *parent, const std::string &label);
    const std::string &label() const { return mLabel; }

protected:
    std::string mLabel;
    Color mTextColor;
    bool mConnected;

    // Internal helper function to maintain nested window position values
    virtual void refreshRelativePlacement();
};
