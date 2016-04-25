#include <src/stringhelper.h>
#include <src/Jinja2CppLight.h>
#include <nanogui/glutil.h>
#include "common.hpp"
#include "glshadersource.hpp"
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
void OpenGLShader::addShaderSource(GLShaderSource *shaderSource)
{
    spdlog::get("qde")->debug(
        "Shader: Trying to add shader source: {}",
        shaderSource->name()
    );
    
    auto source = mShaderSources.find(shaderSource->name());
    
    if (source == mShaderSources.end()) {
        mShaderSources[shaderSource->name()] = shaderSource;
        spdlog::get("qde")->debug(
            "Shader: Successfully added shader source: {}",
            shaderSource->name()
        );
    }
    else {
        spdlog::get("qde")->debug(
            "Shader: Not added shader source as already added ({} times): {}",
            shaderSource->name(), shaderSource->timesUsed()
        );
    }
}

// When connecting node
void OpenGLShader::addShaderObject(GLShaderObject *shaderObject)
{
    auto object = mShaderObjects.find(shaderObject->name());

    if (object == mShaderObjects.end()) {
        // Add definition to source
        mFragmentShaderObjects = fmt::format(
            "{}\n{}",
            mFragmentShaderObjects,
            shaderObject->definition()
        );
        // Add uniform(s) to source
        mFragmentShaderUniforms = fmt::format(
            "{}\n{}",
            mFragmentShaderUniforms,
            shaderObject->uniforms()
        );
        // This is needed for binding the uniforms later on
        mShaderObjects.insert(std::make_pair(shaderObject->name(), shaderObject));
        spdlog::get("qde")->debug("Shader: Added ShaderObject {} to output", shaderObject->name());
    }
    else {
        spdlog::get("qde")->warn(
            "Shader: ShaderObject {} is already added, not adding to output",
            shaderObject->name()
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
    for (auto shaderObjectsIter = mShaderObjects.begin(); shaderObjectsIter != mShaderObjects.end(); ++shaderObjectsIter) {
        auto shaderObject = shaderObjectsIter->second;
        for (auto paramItr = shaderObject->parameters().begin(); paramItr != shaderObject->parameters().end(); ++paramItr) {
            const auto &parameter = *paramItr;
            parameter->setUniform(this);
        }
    }
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