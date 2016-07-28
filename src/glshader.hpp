#pragma once

#include <nanogui/glutil.h>

NAMESPACE_BEGIN(QCE);

class GLShaderSource;
class GLShaderObject;


class OpenGLShader : public nanogui::GLShader
{
public:
    OpenGLShader();
    void setName(const std::string &name) { mName = name; }
    void setFragmentShaderTemplate(const std::string &shaderTemplate) { mFragmentShaderTemplate = shaderTemplate; }
    const std::string &fragmentShaderSource() const { return mFragmentShaderSource; }
    void addShaderSource(GLShaderSource *shaderSource);
    void setFragmentShaderCalls(const std::string &calls) { mFragmentShaderCalls = calls; }
    const std::string &fragmentShaderCalls() const { return mFragmentShaderCalls; }
    void setFragmentShaderCamera(const std::string &camera) { mFragmentShaderCamera = camera; }
    const std::string &fragmentShaderCamera() const { return mFragmentShaderCamera; }
    void setVertexShaderSource(const std::string &vertexSource) { mVertexShaderSource = vertexSource; }
    void setGeometryShaderSource(const std::string &geometrySource) { mGeometryShaderSource = geometrySource; }
    void addShaderObject(GLShaderObject *shaderObject);
    bool recompile();
    void setUniforms();

protected:
    std::string mFragmentShaderTemplate;
    std::string mFragmentShaderUniforms;
    std::string mFragmentShaderObjects;
    std::string mFragmentShaderCalls;
    std::string mFragmentShaderCamera;
    std::string mFragmentShaderCameraOrigin;
    std::string mFragmentShaderCameraTarget;
    std::string mFragmentShaderCameraRoll;
    std::string mFragmentShaderSource;
    std::string mVertexShaderSource;
    std::string mGeometryShaderSource;
    // TODO: Check if list is properly cleared. Use ref maybe?
    std::map<std::string, GLShaderSource *> mShaderSources;
    std::map<std::string, GLShaderObject *> mShaderObjects;

    void parseFragmentShaderTemplate();
};

NAMESPACE_END(QCE);
