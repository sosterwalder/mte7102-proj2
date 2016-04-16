#include "main.hpp"

GraphNodeLink::GraphNodeLink(Widget *parent) :
    Widget(parent),
    mSource(nullptr),
    mSink(nullptr),
    mTargetPosition(Vector2i::Zero())
{
    spdlog::get("qde")->debug("Link: Constructor without connectors");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Source *source) :
    Widget(parent),
    mSource(source),
    mSink(nullptr)
{
    mTargetPosition = source->absolutePosition();
    spdlog::get("qde")->debug("Link: Constructor with source");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Source *source, Sink *sink) :
    Widget(parent),
    mSource(source),
    mSink(sink),
    mTargetPosition(sink->position())
{
    spdlog::get("qde")->debug("Link: Constructor with both sinks");
}

void GraphNodeLink::setTargetPosition(const Vector2i &pos)
{

    mTargetPosition = pos;
}

void GraphNodeLink::draw(NVGcontext* ctx)
{
    Vector2i inputPosition(
        mSource->absolutePosition().x() - mParent->absolutePosition().x(),
        mSource->absolutePosition().y() - mParent->absolutePosition().y()
    );
    Vector2i outputPosition(Vector2i::Zero());

    if (hasTarget()) {
        // Get relative position of parent (node) of the target (sink)
        Vector2i delta = mSink->parent()->absolutePosition() - mSink->parent()->position();
        outputPosition = mSink->absolutePosition() - delta;
    }
    else {
        Vector2i offset = mSource->absolutePosition() - mParent->absolutePosition();
        Vector2i delta = mTargetPosition - mSource->position();
        outputPosition = offset + delta;
    }

    Vector2i positionDiff = outputPosition - inputPosition;

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
