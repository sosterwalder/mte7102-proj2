#pragma once

#include "main.hpp"

class GraphNodeLink : public Widget
{
public:
    GraphNodeLink(Widget *parent);
    GraphNodeLink(Widget *parent, Source *source);
    GraphNodeLink(Widget *parent, Source *source, Sink *sink);
    void setSource(const ref<Source> source) { mSource = source; }
    Source *source() { return mSource; }
    const Source *source() const { return mSource.get(); }
    void setSink(const ref<Sink> sink) { mSink = sink; }
    Sink *sink() { return mSink; }
    const Sink *sink() const { return mSink.get(); }
    void setTargetPosition(const Vector2i &pos);
    virtual void draw(NVGcontext* ctx);

protected:
    ref<Source> mSource;
    ref<Sink> mSink;
    Vector2i  mTargetPosition;

    bool hasTarget() { return mSink != nullptr; }
};
