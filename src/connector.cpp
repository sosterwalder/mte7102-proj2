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

Connector::Connector(nanogui::Widget *parent, Graph *parentGraph, const std::string &label) :
    nanogui::Widget(parent),
    mParentGraph(parentGraph),
    mLink(nullptr),
    mLabel(label),
    mTextColor(nanogui::Color(0, 0)),
    mDrag(false),
    mRelativePosition(Eigen::Vector2i::Zero())
{
    mFixedSize = Eigen::Vector2i(15, 15);
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
    nvgText(ctx, mPos.x() + 1.2f * mSize.y(), mPos.y() + mSize.y() * 0.1f, mLabel.c_str(), nullptr);

    // Draw body
    nvgBeginPath(ctx);
    nvgCircle(ctx, center.x(), center.y(), radius);
    // nvgRect(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y());
    nvgFillColor(ctx, mTheme->mBorderMedium);
    nvgStrokeColor(ctx, mTheme->mBorderLight);
    nvgStroke(ctx);
    nvgFill(ctx);
    
    if (isConnected()) {
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
        "Connector '{}': Received mouse button event at ({},{})",
        mLabel, p.x(), p.y()
    );

    if (Widget::mouseButtonEvent(p, button, down, modifiers)) {
        return true;
    }

    if (button == GLFW_MOUSE_BUTTON_1) {
        bool wasDragged = mDrag;
        Connector *activeConnector = mParentGraph->activeConnector();

        mDrag = down && (p.y() - mPos.y()) < mTheme->mWindowHeaderHeight;

        if (!isConnected() && !wasDragged && mDrag) {
            mLink = new GraphNodeLink(mParentGraph, this);
            mLink->setTargetPosition(p);
            spdlog::get("qde")->debug(
                "Connector '{}': Created a new link ({},{})",
                mLabel, p.x(), p.y()
            );
        }

        if (mDrag) {
            mParentGraph->setActiveConnector(this);
            spdlog::get("qde")->debug(
                "Connector '{}': Set as active",
                mLabel
            );
        }
        else if (activeConnector && activeConnector != this) {
            // (this) Connector is the target
            // activeConnector is the source
            spdlog::get("qde")->debug(fmt::format(
                "Connection from {} -> {}",
                this->label(), activeConnector->label()
            ));
            spdlog::get("qde")->debug("Connector '{}': setting active Connector '{}'", mLabel, (void*)activeConnector);

            // Get source link
            mLink = activeConnector->link();
            mLink->setSink(dynamic_cast<Sink*>(this));
            mLink->setId(fmt::format(
                "{}-{}",
                activeConnector->id(), this->id()
            ));
            // TODO: Fix this, this feels horribly wrong!
            mParentGraph->nodeConnectedEvent(dynamic_cast<GraphNode*>(activeConnector->parent()));
            mParentGraph->calculateOutput();
        }
        // TODO: Fix this
        /*
        else if (mLink != nullptr) {
            spdlog::get("qde")->debug(
                "Connector '{}': Got no connection, deleting link",
                mLabel
            );
            // TODO: Check if this really is the proper way to delete an
            // object. What happens with the instance here?
            mParentGraph->removeChild(mLink.get());
            mLink = nullptr;
        }
        */

        spdlog::get("qde")->debug(
            "Connector '{}': Was dragged: {} - {} ({},{})",
            mLabel, wasDragged, mDrag, p.x(), p.y()
        );

        return true;
    }

    return false;
}


bool Connector::mouseDragEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers)
{
    /*
    spdlog::get("qde")->debug(
        "Connector '{}': Getting dragged: ({},{})",
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

bool Connector::mouseEnterEvent(const Eigen::Vector2i &p, bool enter)
{
    spdlog::get("qde")->debug(
        "Connector '{}': Mouse entered at ({},{})",
        mLabel, p.x(), p.y()
    );
    return Widget::mouseEnterEvent(p, enter);
}

NAMESPACE_END(QCE);