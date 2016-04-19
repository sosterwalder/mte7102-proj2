#pragma once

#include <nanogui/window.h>

NAMESPACE_BEGIN(QCE);

class DirectPopup;
class Sink;
class Source;
class GLShaderObject;


class GraphNode : public nanogui::Window
{
public:
    GraphNode(Widget *parent, const std::string &title = "Undefined");
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
    void setShaderObject(GLShaderObject *shaderObject) { mShaderObject = shaderObject; }
    nanogui::ref<GLShaderObject> shaderObject() { return mShaderObject; }
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual std::string calculateOutput();

protected:
    nanogui::ref<DirectPopup> mPopup;
    int mNumInputs, mNumOutputs;
    bool mDrag;
    std::vector<Sink *> mInputs;
    std::vector<Source *> mOutputs;
    Eigen::Vector2i mAnchorPos;
    int mAnchorHeight;
    nanogui::ref<GLShaderObject> mShaderObject;

    virtual void refreshRelativePlacement() {};
};

NAMESPACE_END(QCE);