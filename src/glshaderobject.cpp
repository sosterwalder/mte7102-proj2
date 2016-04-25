#include <src/pugiconfig.hpp>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/textbox.h>
#include "common.hpp"
#include "qce.hpp"
#include "util.hpp"
#include "glshadersource.hpp"
#include "glshaderparameter.hpp"
#include "glshaderobject.hpp"


NAMESPACE_BEGIN(QCE);

GLShaderObject::GLShaderObject(GLShaderSource *shaderSource, Qce *screen) :
    mHasProperties(false),
    mShaderSource(shaderSource)
{
    mId = mShaderSource->timesUsed();
    mName = fmt::format(mShaderSource->nameTemplate(), mId);
    
    spdlog::get("qde")->debug("ShaderObject {}: Getting constructed", mName, (void*)this);
    
    mWindow = new nanogui::Window(screen, fmt::format("{} Properties", mName));
    mWindow->setPosition(Eigen::Vector2i(10, 10));
    mWindow->setVisible(false);
    nanogui::ref<nanogui::GridLayout> gridLayout = new nanogui::GridLayout(
        nanogui::Orientation::Horizontal, 2,
        nanogui::Alignment::Middle, 15, 5
    );
    gridLayout->setColAlignment({nanogui::Alignment::Maximum, nanogui::Alignment::Fill});
    gridLayout->setSpacing(0, 10);
    mWindow->setLayout(gridLayout);
}

std::string GLShaderObject::uniforms()
{
    fmt::MemoryWriter out;
    
    size_t remainingParamters = mParameters.size();

    for (auto paramItr = mParameters.begin(); paramItr != mParameters.end(); ++paramItr) {
        std::unique_ptr<GLShaderParameter> &parameter = *paramItr;
        
        if (parameter->parameterType() == ParameterType::PROPERTY) {
            out << "uniform ";
            out << GLShaderSource::builtinString(parameter->builtinType());
            out << " ";
            out << parameter->name();
            out << ";";
        }

        // If we are not at the last parameter, add a new line
        if (remainingParamters-- != 1) {
            out << "\n";
        }
    }

    return out.str();
}

std::string GLShaderObject::call()
{
    fmt::MemoryWriter out;
    size_t remainingParamters = mParameters.size();
    
    out << mShaderSource->functionName() << "(";

    for (auto paramItr = mParameters.begin(); paramItr != mParameters.end(); ++paramItr) {
        std::unique_ptr<GLShaderParameter> &parameter = *paramItr;

        out << parameter->call();

        // If we are not at the last parameter add a comma for separatation
        if (remainingParamters-- != 1) {
            out << ", ";
        }
    }
    out << ")";

    spdlog::get("qde")->debug("ShaderObject {}: Calculated output", name());

    return out.str();
}

GLShaderParameter *GLShaderObject::addParameter(const GLShaderObjectParameter &parameter)
{
    spdlog::get("qde")->debug("ShaderObject {}: Found parameter {}", mName, parameter.name);

    switch (parameter.parameterType) {
        case ParameterType::PROPERTY:
            return addProperty(parameter);
            break;

        case ParameterType::INPUT:
            return addInput(parameter);
            break;

        default:
            throw new std::runtime_error(fmt::format("Unknown parameter type: {}", parameter.name));
            break;
    }

    spdlog::get("qde")->debug("ShaderObject {}: Added parameter: {}", mName, parameter.name);
}

GLShaderParameter *GLShaderObject::addProperty(const GLShaderObjectParameter &parameter)
{
    spdlog::get("qde")->debug("ShaderObject {}: Adding property parameter {}", mName, parameter.name);
    
    mHasProperties = true;
    
    switch (parameter.builtinType) {
        case BuiltinType::FLOAT:
            return addFloatProperty(parameter);
            break;

        case BuiltinType::VEC3:
            return addVector3fProperty(parameter);
            break;

        default:
            throw new std::runtime_error("Unknown builtin type");
            break;
    }
}

GLShaderParameter *GLShaderObject::addFloatProperty(const GLShaderObjectParameter &parameter)
{

    GLShaderFloatParameter *floatParam = new GLShaderFloatParameter();
    auto name = fmt::format(parameter.name, mId);
    floatParam->setName(name);
    floatParam->setCall(fmt::format(parameter.call, mId));
    floatParam->setBuiltinType(parameter.builtinType);
    floatParam->setParameterType(parameter.parameterType);

    std::unique_ptr<GLShaderFloatParameter> ptr(floatParam);
    mParameters.push_back(std::move(ptr));

    nanogui::ref<nanogui::Label> label     = new nanogui::Label(mWindow, name);
    nanogui::ref<nanogui::TextBox> textBox = new nanogui::TextBox(mWindow);
    textBox->setEditable(true);
    textBox->setFixedSize(Eigen::Vector2i(100, 20));
    textBox->setValue(std::to_string((float) floatParam->data()));
    textBox->setDefaultValue("0.0");
    textBox->setFontSize(16);
    textBox->setCallback([floatParam](const std::string &value) {
        floatParam->setData(std::stof(value));
        return true;
    });

    // TODO: OK? delete floatParam;
    return floatParam;
}

GLShaderParameter *GLShaderObject::addVector3fProperty(const GLShaderObjectParameter &parameter)
{
    GLShaderVector3fParameter *vec3fParam = new GLShaderVector3fParameter();
    auto name = fmt::format(parameter.name, mId);
    vec3fParam->setName(name);
    vec3fParam->setCall(fmt::format(parameter.call, mId));
    vec3fParam->setBuiltinType(parameter.builtinType);
    vec3fParam->setParameterType(parameter.parameterType);

    std::unique_ptr<GLShaderVector3fParameter> ptr(vec3fParam);
    mParameters.push_back(std::move(ptr));

    new nanogui::Label(mWindow, fmt::format("{}.x", name));
    nanogui::ref<nanogui::TextBox> textBox = new nanogui::TextBox(mWindow);
    textBox->setEditable(true);
    textBox->setFixedSize(Eigen::Vector2i(100, 20));
    textBox->setValue(std::to_string((float) vec3fParam->data().x()));
    textBox->setDefaultValue("0.0");
    textBox->setFontSize(16);
    textBox->setCallback([vec3fParam](const std::string &value) {
        Eigen::Vector3f currentPosition = vec3fParam->data();
        float xPosition = std::stof(value);

        vec3fParam->setData(Eigen::Vector3f(
            xPosition,
            currentPosition.y(),
            currentPosition.z()
        ));

        return true;
    });
    new nanogui::Label(mWindow, fmt::format("{}.y", name));
    textBox = new nanogui::TextBox(mWindow);
    textBox->setEditable(true);
    textBox->setFixedSize(Eigen::Vector2i(100, 20));
    textBox->setValue(std::to_string((float) vec3fParam->data().y()));
    textBox->setDefaultValue("0.0");
    textBox->setFontSize(16);
    textBox->setCallback([vec3fParam](const std::string &value) {
        Eigen::Vector3f currentPosition = vec3fParam->data();
        float yPosition = std::stof(value);

        vec3fParam->setData(Eigen::Vector3f(
            currentPosition.x(),
            yPosition,
            currentPosition.z()
        ));

        return true;
    });
    new nanogui::Label(mWindow, fmt::format("{}.z", name));
    textBox = new nanogui::TextBox(mWindow);
    textBox->setEditable(true);
    textBox->setFixedSize(Eigen::Vector2i(100, 20));
    textBox->setValue(std::to_string((float) vec3fParam->data().z()));
    textBox->setDefaultValue("0.0");
    textBox->setFontSize(16);
    textBox->setCallback([vec3fParam](const std::string &value) {
        Eigen::Vector3f currentPosition = vec3fParam->data();
        float zPosition = std::stof(value);

        vec3fParam->setData(Eigen::Vector3f(
            currentPosition.x(),
            currentPosition.y(),
            zPosition
        ));

        return true;
    });

    // TODO: OK? delete vec3fParam;
    return vec3fParam;
}

GLShaderParameter *GLShaderObject::addInput(const GLShaderObjectParameter &parameter)
{
    spdlog::get("qde")->debug("ShaderObject {}: Adding input parameter {}", mName, parameter.name);
    
    GLShaderInputParameter *inputParam = new GLShaderInputParameter();
    auto name = fmt::format(parameter.name, mId);
    inputParam->setName(name);
    inputParam->setCall(fmt::format(parameter.call, mId));
    inputParam->setBuiltinType(parameter.builtinType);
    inputParam->setParameterType(parameter.parameterType);

    std::unique_ptr<GLShaderInputParameter> ptr(inputParam);
    mParameters.push_back(std::move(ptr));
    
    // TODO: OK? delete inputParam;
    return inputParam;
}

NAMESPACE_END(QCE);