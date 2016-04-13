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
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual std::string calculateOutput();

protected:
    DirectPopup *mPopup;
    int mNumInputs, mNumOutputs;
    bool mDrag;
    std::vector<Sink *> mInputs;
    std::vector<Source *> mOutputs;
    Vector2i mAnchorPos;
    int mAnchorHeight;
    ref<GLShaderObject> mShaderObject;

    virtual void refreshRelativePlacement() {};
    Vector2i getSlotPosition(const float offset) const;
};
