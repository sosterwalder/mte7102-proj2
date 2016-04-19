#include <nanogui/common.h>
#include <nanogui/label.h>
#include <nanogui/opengl.h>
#include <nanogui/theme.h>
#include "common.hpp"
#include "clickablelabel.hpp"

NAMESPACE_BEGIN(QCE);

ClickableLabel::ClickableLabel(
    Widget *parent, const std::string &caption,
    const std::string &font, int fontSize
) : Label(parent, caption, font, fontSize), mPushed(false)
{
}

bool ClickableLabel::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "ClickableLabel '{}': Received click at ({},{})",
        mCaption, p.x(), p.y()
    );

    Widget::mouseButtonEvent(p, button, down, modifiers);

    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */
    nanogui::ref<ClickableLabel> self = this;

    if (button == GLFW_MOUSE_BUTTON_1 && mEnabled) {
        if (down) {
            mPushed = true;
        }
        else if (mPushed) {
            if (contains(p) && mCallback) {
                Eigen::Vector2i parentPos = this->parent()->position();
                mCallback(parentPos);
            }
            mPushed = false;
        }

        return true;
    }

    return false;
}

void ClickableLabel::draw(NVGcontext *ctx)
{
    Label::draw(ctx);

    if (mPushed) {
        nvgFontSize(ctx, mFontSize);
        nvgFontFace(ctx, "sans-bold");
        nvgFillColor(ctx, mTheme->mTextColorShadow);

        if (mFixedSize.x() > 0) {
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgText(ctx, mPos.x(), mPos.y(), mCaption.c_str(), nullptr);
        }
        else {
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgText(ctx, mPos.x(), mPos.y() + 1.0f + mSize.y() * 0.5f, mCaption.c_str(), nullptr);
        }
    }
    else if (mMouseFocus && mEnabled) {
        if (mFixedSize.x() > 0) {
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgText(ctx, mPos.x(), mPos.y(), mCaption.c_str(), nullptr);
        }
        else {
            nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgText(ctx, mPos.x(), mPos.y() + 0.5f + mSize.y() * 0.5f, mCaption.c_str(), nullptr);
        }
    }
}

NAMESPACE_END(QCE);