#pragma once

#include "main.hpp"

class OpenGLShader : public nanogui::GLShader
{
public:
    OpenGLShader();
    void setName(const std::string &name) { mName = name; }
    void setFragmentShaderTemplate(const std::string &shaderTemplate) { mFragmentShaderTemplate = shaderTemplate; }
    const std::string &fragmentShaderSource() const { return mFragmentShaderSource; }
    void addShaderObject(GLShaderObject *object);
    const std::map<std::string, GLShaderObject *> &shaderObjects() const { return mShaderObjects; }
    void setFragmentShaderCalls(const std::string &calls) { mFragmentShaderCalls = calls; }
    const std::string &fragmentShaderCalls() const { return mFragmentShaderCalls; }
    void setVertexShaderSource(const std::string &vertexSource) { mVertexShaderSource = vertexSource; }
    void setGeometryShaderSource(const std::string &geometrySource) { mGeometryShaderSource = geometrySource; }

    void addShaderObjectToOutput(const std::string &id);
    bool recompile();

protected:
    std::string mFragmentShaderTemplate;
    std::string mFragmentShaderUniforms;
    std::string mFragmentShaderObjects;
    std::string mFragmentShaderCalls;
    std::string mFragmentShaderSource;
    std::string mVertexShaderSource;
    std::string mGeometryShaderSource;
    std::map<std::string, GLShaderObject *> mShaderObjects;
    std::vector<std::string> mShaderObjectsOutput;

    void parseFragmentShaderTemplate();
};
