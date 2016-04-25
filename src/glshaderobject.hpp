#pragma once

// System imports
#include <nanogui/object.h>
#include <nanogui/window.h>
#include <src/pugixml.hpp>
#include "glshadersource.hpp"
#include "glshaderparameter.hpp"


NAMESPACE_BEGIN(QCE);

class Qce;
class GLShaderSource;
struct GLShaderObjectParameter;

class GLShaderObject : public nanogui::Object
{
public:
    GLShaderObject(GLShaderSource *shaderSource, Qce *screen);
    void setName(const std::string &name) { mName = name; }
    const std::string &name() const { return mName; }
    std::string uniforms();
    const std::vector<std::unique_ptr<GLShaderParameter>> &parameters() { return mParameters; }
    const nanogui::ref<GLShaderSource> shaderSource() { return mShaderSource; }
    GLShaderParameter *addParameter(const GLShaderObjectParameter &parameter);
    void showForm() { mWindow->setVisible(true); }
    void hideForm() { mWindow->setVisible(false); }
    const std::string &definition() { return mShaderSource->definition(); }
    std::string call();
    const bool &hasProperties() const { return mHasProperties; }

protected:
    std::string mName;
    bool mHasProperties;
    std::vector<std::unique_ptr<GLShaderParameter>> mParameters;
    nanogui::ref<nanogui::Window> mWindow;
    nanogui::ref<nanogui::Widget> mPanel;
    nanogui::ref<GLShaderSource> mShaderSource;

private:
    int mId;
    
    GLShaderParameter *addProperty(const GLShaderObjectParameter &parameter);
    GLShaderParameter *addFloatProperty(const GLShaderObjectParameter &parameter);
    GLShaderParameter *addVector3fProperty(const GLShaderObjectParameter &parameter);
    GLShaderParameter *addInput(const GLShaderObjectParameter &parameter);
};

NAMESPACE_END(QCE);
