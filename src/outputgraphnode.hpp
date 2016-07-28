#pragma once


NAMESPACE_BEGIN(QCE);

class OutputGraphNode : public GraphNode
{
public:
    OutputGraphNode(Widget *parent, Graph *parentGraph, const std::string &title = "Undefined");
    Sink *cameraSink() { return mCameraSink; }
    const Sink *cameraSink() const { return mCameraSink.get(); }

    virtual void performLayout(NVGcontext *ctx);

protected:
    nanogui::ref<Sink> mOutputSink;
    nanogui::ref<Sink> mCameraSink;
};

NAMESPACE_END(QCE);
