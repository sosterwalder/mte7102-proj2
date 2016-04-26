#pragma once

#include <nanogui/window.h>

NAMESPACE_BEGIN(QCE);

class DirectPopup;
class Sink;
class Source;
class GLShaderSource;
class GLShaderObject;


class GraphNode : public nanogui::Window
{
public:
    GraphNode(Widget *parent, Graph *parentGraph, const std::string &title = "Undefined");
    // TODO: Maybe implement destructor? And decrease shader source usage count?
    void setAnchorPos(const Eigen::Vector2i &anchorPos) { mAnchorPos = anchorPos; }
    const Eigen::Vector2i &anchorPos() const { return mAnchorPos; }
    void setAnchorHeight(int anchorHeight) { mAnchorHeight = anchorHeight; }
    int anchorHeight() const { return mAnchorHeight; }
    const std::vector<Sink *> &inputs() const { return mInputs; }
    const std::vector<Source *> &outputs() const { return mOutputs; }
    virtual void draw(NVGcontext* ctx);
    void addSink(Sink *sink);
    void removeSink(const int index);
    void removeSink(const Sink *sink);
    void addSource(Source *source);
    void removeSource(const int index);
    void removeSource(const Source *source);
    void setShaderSource(GLShaderSource *shaderSource) { mShaderSource = shaderSource; }
    nanogui::ref<GLShaderSource> shaderSource() { return mShaderSource; }
    void setShaderObject(GLShaderObject *shaderObject) { mShaderObject = shaderObject; }
    nanogui::ref<GLShaderObject> shaderObject() { return mShaderObject; }
    int inputCount() { return mInputs.size(); }
    int outputCount() { return mOutputs.size(); }
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual bool sinkConnectedEvent(Connector *sink);
    virtual std::string calculateOutput();

protected:
    nanogui::ref<Graph> mParentGraph;
    nanogui::ref<DirectPopup> mPopup;
    int mNumInputs, mNumOutputs;
    bool mDrag;
    std::vector<Sink *> mInputs;
    std::vector<Source *> mOutputs;
    Eigen::Vector2i mAnchorPos;
    int mAnchorHeight;
    nanogui::ref<GLShaderSource> mShaderSource;
    nanogui::ref<GLShaderObject> mShaderObject;

    virtual void refreshRelativePlacement() {};
};

NAMESPACE_END(QCE);