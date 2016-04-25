#pragma once

#include <nanogui/screen.h>
#include "glshader.hpp"

NAMESPACE_BEGIN(QCE);

class GraphNode;
class Graph;
class GLShaderSource;
class GLShaderObject;


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
    nanogui::ref<Graph> mNodeGraph;
    std::vector<nanogui::ref<GLShaderSource>> mShaderSources;
    // TODO: Check if list gets properly deleted. Use ref maybe?
    std::vector<GraphNode *> mNodeTypes;
    float mIntensity;
    double mStartTime;
    double mUpdateTime;
    int mNumFrames;

    void findAndAddShaderFiles(const std::string &path);
    void initializeShaderOperations();
    void initializeShaderObjects();
    void initializeShader();
};

NAMESPACE_END(QCE);
