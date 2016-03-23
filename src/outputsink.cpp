#include "main.hpp"

OutputSink::OutputSink(Widget *parent, const std::string &label) :
    Sink(parent, label)
{
}

void OutputSink::refreshRelativePlacement()
{
    spdlog::get("qde")->debug(
        "OutputSink '{}': Refreshing rel. placement",
        mLabel
    );
    spdlog::get("qde")->debug(
        "OutputSink '{}': Parent pos: ({},{})",
        mLabel, mParent->position().x(), mParent->position().y()
    );
    mPos = Vector2i(
        mParent->size().x() - mSize.x() - 10,
        mParent->size().y() / 2 - mSize.y() / 2
    );
}

void OutputSink::draw(NVGcontext* ctx)
{
    refreshRelativePlacement();

    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx,
                 mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(ctx, mPos.x() - 1.2f * mSize.y() - mLabel.length(), mPos.y() + mSize.y() * 0.5f,
            mLabel.c_str(), nullptr);

    NVGpaint bg = nvgBoxGradient(ctx, mPos.x() + 1.5f, mPos.y() + 1.5f,
                                 mSize.y() - 2.0f, mSize.y() - 2.0f, 3, 3,
                                 Color(0, 32),
                                 Color(0, 0, 0, 180));

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 1.0f, mPos.y() + 1.0f, mSize.y() - 2.0f,
                   mSize.y() - 2.0f, 3);
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    if (mConnected) {
        nvgFontSize(ctx, 1.8 * mSize.y());
        nvgFontFace(ctx, "icons");
        nvgFillColor(ctx, mEnabled ? mTheme->mIconColor
                                   : mTheme->mDisabledTextColor);
        nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(ctx, mPos.x() + mSize.y() * 0.5f + 1,
                mPos.y() + mSize.y() * 0.5f, utf8(ENTYPO_ICON_CHECK).data(),
                nullptr);
    }

    Sink::draw(ctx);
}
