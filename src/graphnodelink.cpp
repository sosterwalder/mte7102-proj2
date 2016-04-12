#include "main.hpp"

GraphNodeLink::GraphNodeLink(Widget *parent) :
    Widget(parent),
    mSource(nullptr),
    mTarget(nullptr),
    mTargetPosition(Vector2i::Zero())
{
    spdlog::get("qde")->debug("Link: Constructor without sinks");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Sink *source) :
    Widget(parent),
    mSource(source),
    mTarget(nullptr)
{
    mTargetPosition = source->absolutePosition();
    spdlog::get("qde")->debug("Link: Constructor with source sink");
}

GraphNodeLink::GraphNodeLink(Widget *parent, Sink *source, Sink *target) :
    Widget(parent),
    mSource(source),
    mTarget(target),
    mTargetPosition(target->position())
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
        // Vector2i offset = mTarget->absolutePosition() - mParent->absolutePosition();
        // Vector2i delta = mTarget->position() - mSource->position();
        // outputPosition = offset + delta;
        // outputPosition = Vector2i(
        //     mTarget->absolutePosition().x() - mParent->absolutePosition().x(),
        //     mTarget->absolutePosition().y() - mParent->absolutePosition().y() + 10
        // );
        outputPosition = mTarget->absolutePosition();
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
