#include "main.hpp"

ClickableLabel::ClickableLabel(
    Widget *parent, const std::string &caption,
    const std::string &font, int fontSize
) : Label(parent, caption, font, fontSize), mPushed(false)
{
}

bool ClickableLabel::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    Widget::mouseButtonEvent(p, button, down, modifiers);
    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */
    ref<ClickableLabel> self = this;

    if (button == GLFW_MOUSE_BUTTON_1 && mEnabled) {
        if (down) {
            mPushed = true;
        }
        else if (mPushed) {
            if (contains(p) && mCallback) {
                Vector2i parentPos = this->parent()->position();
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
