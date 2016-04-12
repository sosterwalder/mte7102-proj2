#pragma once

#include "main.hpp"

class GraphNodeLink : public Widget
{
public:
    GraphNodeLink(Widget *parent);
    GraphNodeLink(Widget *parent, Sink *source);
    GraphNodeLink(Widget *parent, Sink *source, Sink *target);
    void setSource(const ref<Sink> source) { mSource = source; }
    Sink *source() { return mSource; }
    const Sink *source() const { return mSource.get(); }
    void setTarget(const ref<Sink> target) { mTarget = target; }
    Sink *target() { return mTarget; }
    const Sink *target() const { return mTarget.get(); }
    void setTargetPosition(const Vector2i &pos);
    virtual void draw(NVGcontext* ctx);

protected:
    ref<Sink> mSource;
    ref<Sink> mTarget;
    Vector2i  mTargetPosition;

    bool hasTarget() { return mTarget != nullptr; }
};
