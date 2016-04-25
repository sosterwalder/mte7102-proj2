#include <spdlog/spdlog.h>
#include <nanovg.h>
#include <nanogui/common.h>
#include "connector.hpp"
#include "graphnode.hpp"
#include "graphnodelink.hpp"


NAMESPACE_BEGIN(QCE);

GraphNodeLink::GraphNodeLink(Widget *parent) :
    Widget(parent),
    mSource(nullptr),
    mSink(nullptr),
    mTargetPosition(Eigen::Vector2i::Zero())
{
    spdlog::get("qde")->debug("Link: Constructor without connectors");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Connector *source) :
    Widget(parent),
    mSource(source),
    mSink(nullptr)
{
    mTargetPosition = source->absolutePosition();
    spdlog::get("qde")->debug("Link: Constructor with source");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Connector *source, Connector *sink) :
    Widget(parent),
    mSource(source),
    mSink(sink),
    mTargetPosition(sink->position())
{
    spdlog::get("qde")->debug("Link: Constructor with both sinks");
}

void GraphNodeLink::setTargetPosition(const Eigen::Vector2i &pos)
{

    mTargetPosition = pos;
}

void GraphNodeLink::draw(NVGcontext* ctx)
{
    auto sourceSize = mSource->size().cast<float>();
    
    Eigen::Vector2i inputPosition(
        mSource->absolutePosition().x() - mParent->absolutePosition().x() + (sourceSize.x() * 0.5f),
        mSource->absolutePosition().y() - mParent->absolutePosition().y() + (sourceSize.y() * 0.5f)
    );
    Eigen::Vector2i outputPosition(Eigen::Vector2i::Zero());

    if (hasTarget()) {
        // Get relative position of parent (node) of the target (sink)
        Eigen::Vector2i delta = mSink->parent()->absolutePosition() - mSink->parent()->position();
        delta.x() -= (sourceSize.x() * 0.5f);
        delta.y() -= (sourceSize.y() * 0.5f);
        outputPosition = mSink->absolutePosition() - delta;
    }
    else {
        Eigen::Vector2i offset = mSource->absolutePosition() - mParent->absolutePosition();
        Eigen::Vector2i delta = mTargetPosition - mSource->position();
        outputPosition = offset + delta;
    }

    Eigen::Vector2i positionDiff = outputPosition - inputPosition;

    NVGcontext* vg = ctx;

    nvgStrokeColor(vg, nvgRGBA(131, 148, 150, 255));
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgMoveTo(
        vg,
        inputPosition.x(),
        inputPosition.y()
    );
    nvgQuadTo(vg,
        1.2 * positionDiff.x(), positionDiff.y(),
        outputPosition.x(), outputPosition.y()
    );
    nvgStroke(vg);

    Widget::draw(ctx);
}

NAMESPACE_END(QCE);