#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(QCE);

class Connector;


class GraphNodeLink : public nanogui::Widget
{
public:
    GraphNodeLink(nanogui::Widget *parent);
    GraphNodeLink(nanogui::Widget *parent, Connector *source);
    GraphNodeLink(nanogui::Widget *parent, Connector *source, Connector *sink);
    void setSource(const nanogui::ref<Connector> source) { mSource = source; }
    Connector *source() { return mSource; }
    const Connector *source() const { return mSource.get(); }
    void setSink(const nanogui::ref<Connector> sink) { mSink = sink; }
    Connector *sink() { return mSink; }
    const Connector *sink() const { return mSink.get(); }
    void setTargetPosition(const Eigen::Vector2i &pos);
    virtual void draw(NVGcontext* ctx);

protected:
    nanogui::ref<Connector> mSource;
    nanogui::ref<Connector> mSink;
    Eigen::Vector2i         mTargetPosition;

    bool hasTarget() { return mSink != nullptr; }
};

NAMESPACE_END(QCE);