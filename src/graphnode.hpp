#pragma once

#include "main.hpp"

class GraphNode : public Window
{
public:
    GraphNode(Widget *parent, const std::string &title = "Undefined");
    void setAnchorPos(const Vector2i &anchorPos) { mAnchorPos = anchorPos; }
    const Vector2i &anchorPos() const { return mAnchorPos; }
    void setAnchorHeight(int anchorHeight) { mAnchorHeight = anchorHeight; }
    int anchorHeight() const { return mAnchorHeight; }
    const std::vector<InputSink *> &inputs() const { return mInputs; }
    const std::vector<OutputSink *> &outputs() const { return mOutputs; }
    virtual void draw(NVGcontext* ctx);
    void addInputSink(InputSink *sink);
    void removeInputSink(const int index);
    void removeInputSink(const InputSink *sink);
    void addOutputSink(OutputSink *sink);
    void removeOutputSink(const int index);
    void removeOutputSink(const OutputSink *sink);
    void setShaderObject(GLShaderObject *shaderObject) { mShaderObject = shaderObject; }
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual std::string calculateOutput();

protected:
    DirectPopup *mPopup;
    int mNumInputs, mNumOutputs;
    bool mDrag;
    std::vector<InputSink *> mInputs;
    std::vector<OutputSink *> mOutputs;
    Vector2i mAnchorPos;
    int mAnchorHeight;
    ref<GLShaderObject> mShaderObject;

    virtual void refreshRelativePlacement() {};
    Vector2i getSlotPosition(const float offset) const;
};
