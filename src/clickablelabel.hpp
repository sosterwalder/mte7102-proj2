#pragma once

#include <nanogui/label.h>

NAMESPACE_BEGIN(QCE);

class ClickableLabel : public nanogui::Label
{
public:
    ClickableLabel(Widget *parent, const std::string &caption,
          const std::string &font = "sans", int fontSize = -1);
    bool pushed() const { return mPushed; }
    void setPushed(bool pushed) { mPushed = pushed; }
    std::function<void(Eigen::Vector2i)> callback() const { return mCallback; }
    void setCallback(const std::function<void(Eigen::Vector2i)> &callback) { mCallback = callback; }

    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual void draw(NVGcontext *ctx);

protected:
    bool mPushed;
    std::function<void(Eigen::Vector2i)> mCallback;
};

NAMESPACE_END(QCE);