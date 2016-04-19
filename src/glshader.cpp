#include <src/stringhelper.h>
#include <src/Jinja2CppLight.h>
#include <nanogui/glutil.h>
#include "common.hpp"
#include "glshaderobject.hpp"
#include "glshader.hpp"


NAMESPACE_BEGIN(QCE);

OpenGLShader::OpenGLShader() :
    mFragmentShaderTemplate(""),
    mFragmentShaderUniforms(""),
    mFragmentShaderObjects(""),
    mFragmentShaderSource(""),
    mVertexShaderSource(""),
    mGeometryShaderSource("")
{
}

// When loading files
void OpenGLShader::addShaderObject(GLShaderObject *object)
{
    spdlog::get("qde")->debug(
        "Shader: Trying to add shader definition: {}",
        object->name()
    );
    auto shaderObject = mShaderObjects.find(object->name());
    if (shaderObject == mShaderObjects.end()) {
        mShaderObjects[object->name()] = object;
        spdlog::get("qde")->debug(
            "Shader: Successfully added shader definition: {}",
            object->name()
        );
    }
    else {
        spdlog::get("qde")->debug(
            "Shader: Not added shader definition as already added ({} times): {}",
            object->name(), object->timesUsed()
        );
    }
}

// When connecting node
void OpenGLShader::addShaderObjectToOutput(const std::string &id)
{
    auto shaderObject = mShaderObjects.find(id);

    if (shaderObject != mShaderObjects.end()) {
        auto id = shaderObject->first;
        auto object = shaderObject->second;

        // Try to add shader to output
        auto itr = std::find(mShaderObjectsOutput.begin(), mShaderObjectsOutput.end(), id);
        if (itr == mShaderObjectsOutput.end()) {
            // Shader was not added yet, so
            // add definition to source
            mFragmentShaderObjects = fmt::format(
                "{}\n{}",
                mFragmentShaderObjects,
                object->definition()
            );
            mShaderObjectsOutput.push_back(id);
            spdlog::get("qde")->debug("Shader: Added ShaderObject {} to output", object->name());
        }
        else {
            spdlog::get("qde")->warn(
                "Shader: ShaderObject {} is already added, not adding to output",
                object->name()
            );
        }

        // Add uniform(s) to source
        mFragmentShaderUniforms = fmt::format(
            "{}\n{}",
            mFragmentShaderUniforms,
            object->uniforms()
        );
        spdlog::get("qde")->debug("Shader: Added uniform defintions of ShaderObject {}", id);
    }
    else {
        spdlog::get("qde")->warn(
            "Shader: ShaderObject {} was not found!",
            id
        );
    }

    // (Re-) Parse template
    parseFragmentShaderTemplate();
}

bool OpenGLShader::recompile()
{

    parseFragmentShaderTemplate();
    spdlog::get("qde")->debug("Shader: Recompiling {}", mName);

    return init(
        mName,
        mVertexShaderSource,
        mFragmentShaderSource,
        mGeometryShaderSource
    );
}

void OpenGLShader::setUniforms()
{
    // TODO: Refactor this! Seriously! SERIOUSLY!
    // for (const std::string &shaderObjectId : mShaderObjectsOutput) {
        // const auto &shaderObject = mShaderObjects.at(shaderObjectId);
        // for (auto parameter: shaderObject->parameters()) {
        // }
 
        // TODO: Fix this
        // for (auto &parameter: shaderObject->parameters()) {
        //     parameter.setUniform(this);
        //         
        //     // TODO: Put this in a function on shader object
        //     if (parameter.builtinType() == BuiltinType::FLOAT) {
        //         setUniform(parameter.name(), boost::get<float>(parameter.data()));
        //     }
        //     else if (parameter.builtinType() == BuiltinType::VEC3) {
        //         setUniform(parameter.name(), boost::get<Vector3f>(parameter.data()));
        //     }
        //     else {
        //         throw new std::runtime_error("Unknown bultin type");
        //     }

        //     spdlog::get("qde")->debug(
        //         "Shader: Set uniform: {}",
        //         parameter.name()
        //     );
        // }
    // }
}

void OpenGLShader::parseFragmentShaderTemplate()
{
    Jinja2CppLight::Template shaderTemplate(mFragmentShaderTemplate);
    shaderTemplate.setValue("uniform_variables",  mFragmentShaderUniforms);
    shaderTemplate.setValue("object_definitions", mFragmentShaderObjects);
    shaderTemplate.setValue("calls",              mFragmentShaderCalls);
    mFragmentShaderSource = shaderTemplate.render();
}

NAMESPACE_END(QCE);