#pragma once

#include "main.hpp"

using namespace nanogui;

class ClickableLabel : public Label
{
public:
    ClickableLabel(Widget *parent, const std::string &caption,
          const std::string &font = "sans", int fontSize = -1);
    bool pushed() const { return mPushed; }
    void setPushed(bool pushed) { mPushed = pushed; }
    std::function<void(Vector2i)> callback() const { return mCallback; }
    void setCallback(const std::function<void(Vector2i)> &callback) { mCallback = callback; }

    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual void draw(NVGcontext *ctx);

protected:
    bool mPushed;
    std::function<void(Vector2i)> mCallback;
};
