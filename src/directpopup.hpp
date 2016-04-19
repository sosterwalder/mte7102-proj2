#pragma once

#include <nanogui/popup.h>


NAMESPACE_BEGIN(QCE);

class DirectPopup : public nanogui::Popup
{
public:
    DirectPopup(Widget *parent, Window *parentWindow) :
        Popup(parent, parentWindow),
        mXOffset(20)
    {}

    void setXOffset(int xOffset) { mXOffset = xOffset; }
    void setAnchorPos(const Eigen::Vector2i &anchorPos) { mAnchorPos = anchorPos + Eigen::Vector2i(mXOffset, 0); }
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);

protected:
    /// Internal helper function to maintain nested window position values
    virtual void refreshRelativePlacement();

private:
    int mXOffset;
};

NAMESPACE_END(QCE);