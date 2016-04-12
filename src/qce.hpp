#pragma once

#include "main.hpp"

class Qce : public nanogui::Screen {

public:
    Qce();
    ~Qce();

    virtual void draw(NVGcontext *ctx);
    virtual void drawContents();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    void setIntensity(const float intensity) { mIntensity = intensity; }
    void bindShader();
    const std::vector<GraphNode *> &nodeTypes() const { return mNodeTypes; }
    void addShaderToOutput(GLShaderObject *shaderObject);
    void setShaderOutput(const std::string &output);


private:
    OpenGLShader mShader;
    ref<Graph> mNodeGraph;
    std::vector<GraphNode *> mNodeTypes;
    float mIntensity;
    double mStartTime;
    double mUpdateTime;
    int mNumFrames;

    void initializeShaderObjects();
    void initializeShader();
};
