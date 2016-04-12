#include "main.hpp"

Sink::Sink(Widget *parent, Graph *parentGraph, const std::string &label) :
    Widget(parent),
    mParentGraph(parentGraph),
    mLink(nullptr),
    mLabel(label),
    mTextColor(Color(0, 0)),
    mDrag(false),
    mRelativePosition(Vector2i::Zero())
{
    
}

void Sink::draw(NVGcontext* ctx)
{
    refreshRelativePlacement();

    // Draw label
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(ctx, mPos.x() + 1.2f * mSize.y(), mPos.y() + mSize.y() * 0.1f, mLabel.c_str(), nullptr);

    // Draw body
    Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
    float radius = (int)(mSize.y()*0.5f);
    NVGpaint knobShadow = nvgRadialGradient(ctx, mPos.x(), mPos.y(), radius-3, radius+3, Color(0, 64), mTheme->mTransparent);

    nvgBeginPath(ctx);

    nvgCircle(ctx, mPos.x(), mPos.y(), radius);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, knobShadow);
    nvgFill(ctx);

    NVGpaint knob = nvgLinearGradient(ctx, mPos.x(), center.y() - radius, mPos.x(), center.y() + radius, mTheme->mBorderLight, mTheme->mBorderMedium);
    NVGpaint knobReverse = nvgLinearGradient(ctx, mPos.x(), center.y() - radius, mPos.x(), center.y() + radius, mTheme->mBorderMedium, mTheme->mBorderLight);

    nvgBeginPath(ctx);
    nvgCircle(ctx, mPos.x(), mPos.y(), radius);
    nvgStrokeColor(ctx, mTheme->mBorderDark);
    nvgFillPaint(ctx, knob);
    nvgStroke(ctx);
    nvgFill(ctx);

    if (isConnected()) {
        nvgBeginPath(ctx);
        nvgCircle(ctx, mPos.x(), mPos.y(), radius / 2);
        nvgFillColor(ctx, Color(150, mEnabled ? 255 : 100));
        nvgStrokePaint(ctx, knobReverse);
        nvgStroke(ctx);
        nvgFill(ctx);
    }

    /*
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    if (isConnected()) {
        nvgFontSize(ctx, 1.8 * mSize.y());
        nvgFontFace(ctx, "icons");
        nvgFillColor(ctx, mEnabled ? mTheme->mIconColor
                                   : mTheme->mDisabledTextColor);
        nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(ctx, mPos.x() + mSize.y() * 0.5f + 1,
                mPos.y() + mSize.y() * 0.5f, utf8(ENTYPO_ICON_CHECK).data(),
                nullptr);
    }

    */
    Widget::draw(ctx);
}

Vector2i Sink::preferredSize(NVGcontext *ctx) const
{
    if (mFixedSize != Vector2i::Zero()) {
        return mFixedSize;
    }
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    return Vector2i(
        nvgTextBounds(ctx, 0, 0, mLabel.c_str(), nullptr, nullptr) +
        1.7f * fontSize(),
        fontSize() * 1.3f
    );
}

void Sink::refreshRelativePlacement()
{
    mRelativePosition = relativePosition();
    mPos = mRelativePosition;
}

bool Sink::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "Sink '{}': Received mouse button event at ({},{})",
        mLabel, p.x(), p.y()
    );

    if (Widget::mouseButtonEvent(p, button, down, modifiers)) {
        return true;
    }

    if (button == GLFW_MOUSE_BUTTON_1) {
        bool wasDragged = mDrag;
        Sink *activeSink = mParentGraph->activeSink();

        mDrag = down && (p.y() - mPos.y()) < mTheme->mWindowHeaderHeight;

        if (!isConnected() && !wasDragged && mDrag) {
            mLink = new GraphNodeLink(mParentGraph, this);
            mLink->setTargetPosition(p);
            spdlog::get("qde")->debug(
                "Sink '{}': Created a new link ({},{})",
                mLabel, p.x(), p.y()
            );
        }

        if (mDrag) {
            mParentGraph->setActiveSink(this);
            spdlog::get("qde")->debug(
                "Sink '{}': Set as active",
                mLabel
            );
        }
        else if (activeSink && activeSink != this) {
            spdlog::get("qde")->debug(
                "Sink '{}': Got a connection to '{}'",
                mLabel, activeSink->label()
            );
            spdlog::get("qde")->debug("Sink '{}': setting active sink '{}'", mLabel, (void*)activeSink);
            mLink = activeSink->link();
            mLink->setTarget(activeSink);
            mLink->setId(fmt::format(
                "{}-{}",
                activeSink->id(), this->id()
            ));
            mParentGraph->calculateOutput();
        }
        // TODO: Fix this
        /*
        else if (mLink != nullptr) {
            spdlog::get("qde")->debug(
                "Sink '{}': Got no connection, deleting link",
                mLabel
            );
            // TODO: Check if this really is the proper way to delete an
            // object. What happens with the instance here?
            mParentGraph->removeChild(mLink.get());
            mLink = nullptr;
        }
        */

        spdlog::get("qde")->debug(
            "Sink '{}': Was dragged: {} - {} ({},{})",
            mLabel, wasDragged, mDrag, p.x(), p.y()
        );

        return true;
    }

    return false;
}


bool Sink::mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers)
{
    /*
    spdlog::get("qde")->debug(
        "Sink '{}': Getting dragged: ({},{})",
        mLabel, p.x(), p.y()
    );
    */

    if (Widget::mouseDragEvent(p, rel, button, modifiers)) {
        return true;
    }

    if (mLink) {
        mLink->setTargetPosition(p);

        return true;
    }

    return false;
}

bool Sink::mouseEnterEvent(const Vector2i &p, bool enter)
{
    spdlog::get("qde")->debug(
        "Sink '{}': Mouse entered at ({},{})",
        mLabel, p.x(), p.y()
    );
    return Widget::mouseEnterEvent(p, enter);
}
