#pragma once

#include "main.hpp"

using namespace nanogui;

class DirectPopup : public Popup
{
public:
    DirectPopup(Widget *parent, Window *parentWindow) :
        Popup(parent, parentWindow),
        mXOffset(20)
    {}

    void setXOffset(int xOffset) { mXOffset = xOffset; }
    void setAnchorPos(const Vector2i &anchorPos) { mAnchorPos = anchorPos + Vector2i(mXOffset, 0); }

protected:
    /// Internal helper function to maintain nested window position values
    virtual void refreshRelativePlacement();

private:
    int mXOffset;
};
