#include <nanogui/common.h>
#include <nanogui/opengl.h>
#include <nanogui/theme.h>
#include <nanovg.h>
#include <spdlog/spdlog.h>
#include "graph.hpp"
#include "graphnodelink.hpp"
#include "graphnode.hpp"
#include "connector.hpp"
#include "sink.hpp"


NAMESPACE_BEGIN(QCE);

Connector::Connector(GraphNode *parent, Graph *parentGraph, const std::string &label) :
    nanogui::Widget(parent),
    mParent(parent),
    mParentGraph(parentGraph),
    mLink(nullptr),
    mLabel(label),
    mTextColor(nanogui::Color(0, 0)),
    mIsDragged(false),
    mRelativePosition(Eigen::Vector2i::Zero()),
    mIndex(0)
{
    mFixedSize = mSize = Eigen::Vector2i(15, 15);
}

bool Connector::isConnected()
{
    return mLink != nullptr && mLink->hasTarget();
}

void Connector::draw(NVGcontext* ctx)
{
    refreshRelativePlacement();

    Eigen::Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
    float radius = (int)(mSize.y() * 0.5f);

    // Draw label
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText(ctx, center.x() + radius + 1.2f, center.y(), mLabel.c_str(), nullptr);

    // Draw body
    nvgBeginPath(ctx);
    nvgCircle(ctx, center.x(), center.y(), radius);
    nvgFillColor(ctx, mTheme->mBorderMedium);
    nvgStrokeColor(ctx, mTheme->mBorderLight);
    nvgStroke(ctx);
    nvgFill(ctx);

    if (hasLink()) {
        NVGpaint knobReverse = nvgLinearGradient(ctx, mPos.x(), center.y() - radius, mPos.x(), center.y() + radius, mTheme->mBorderMedium, mTheme->mBorderLight);

        nvgBeginPath(ctx);
        nvgCircle(ctx, center.x(), center.y(), radius * 0.5f);
        nvgFillColor(ctx, nanogui::Color(150, mEnabled ? 255 : 100));
        nvgStrokePaint(ctx, knobReverse);
        nvgStroke(ctx);
        nvgFill(ctx);
    }
    Widget::draw(ctx);
}

Eigen::Vector2i Connector::preferredSize(NVGcontext *ctx) const
{
    if (mFixedSize != Eigen::Vector2i::Zero()) {
        return mFixedSize;
    }
    nvgFontSize(ctx, fontSize());
    nvgFontFace(ctx, "sans");
    return Eigen::Vector2i(
        nvgTextBounds(ctx, 0, 0, mLabel.c_str(), nullptr, nullptr) +
        1.7f * fontSize(),
        fontSize() * 1.3f
    );
}

void Connector::refreshRelativePlacement()
{
    mRelativePosition = relativePosition();
    mPos = mRelativePosition;
    /*
    spdlog::get("qde")->debug(
        "Connector '{}': At ({},{})",
        mLabel, mPos.x(), mPos.y()
    );
    */
}

bool Connector::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    spdlog::get("qde")->debug(
        "Connector '{}': Received mouse button event at ({},{}): {}, {}",
        mLabel, p.x(), p.y(), button, down
    );
    return Widget::mouseButtonEvent(p, button, down, modifiers);

    /*
    // Check if the event gets handled in parent widget
    if (Widget::mouseButtonEvent(p, button, down, modifiers)) {
        return true;
    }

    if (button == GLFW_MOUSE_BUTTON_1) {
        // We want to handle only the events for state changes.
        // A state change is as follows:
        //
        //  * Active:   Not dragged (0) --> dragged (1)
        //  * Inactive: Dragged (1)     --> not dragged (0)
        //
        //     1----1----1
        //    /           \
        //   /             \
        //  0               0
        //
        // The other possible states do not interest us.

        bool wasDraggedBefore = mIsDragged;
        mIsDragged = down && (p.y() - mPos.y()) < mTheme->mWindowHeaderHeight;
        bool currentState = false;

        if (!wasDraggedBefore && mIsDragged) {
            currentState = true;
        }
        else if (wasDraggedBefore && !mIsDragged) {
            currentState = false;
        }
        mParentGraph->connectorDraggedEvent(this, p, currentState);

        return false;
    }

    return false;
    */
}


bool Connector::mouseDragEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers)
{
    spdlog::get("qde")->debug(
        "Connector '{}': Getting dragged: ({},{}), ({},{}), {}, {}",
        mLabel, p.x(), p.y(), rel.x(), rel.y(), button, modifiers
    );
    return Widget::mouseDragEvent(p, rel, button, modifiers);

    /*
    if (Widget::mouseDragEvent(p, rel, button, modifiers)) {
        return true;
    }

    if (mLink) {
        mLink->setTargetPosition(p);
        return true;
    }

    return false;
    */
}

bool Connector::mouseEnterEvent(const Eigen::Vector2i &p, bool enter)
{
    spdlog::get("qde")->debug(
        "Connector '{}': Mouse entered at ({},{})",
        mLabel, p.x(), p.y()
    );
    return Widget::mouseEnterEvent(p, enter);
}

NAMESPACE_END(QCE)
