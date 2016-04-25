#include <src/pugiconfig.hpp>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/textbox.h>
#include "common.hpp"
#include "qce.hpp"
#include "util.hpp"
#include "glshaderparameter.hpp"
#include "glshadersource.hpp"


NAMESPACE_BEGIN(QCE);

std::map<std::string, BuiltinType> GLShaderSource::BUILTIN_STR_TO_TYPE = {
    { "unknown", BuiltinType::UNKNOWN},
    { "float",   BuiltinType::FLOAT  },
    { "vec3",    BuiltinType::VEC3   }
};
std::map<BuiltinType, std::string> GLShaderSource::TYPE_TO_BUILTIN_STR = {
    { BuiltinType::UNKNOWN, "unknown"},
    { BuiltinType::FLOAT,   "float"  },
    { BuiltinType::VEC3,    "vec3"   }
};
std::map<std::string, ParameterType> GLShaderSource::PARAM_STR_TO_TYPE = {
    { "unknown",  ParameterType::UNKNOWN },
    { "property", ParameterType::PROPERTY},
    { "input",    ParameterType::INPUT   }
};
std::map<ParameterType, std::string> GLShaderSource::PARAM_TYPE_TO_STR = {
    { ParameterType::UNKNOWN,  "unknown"  },
    { ParameterType::PROPERTY, "property" },
    { ParameterType::INPUT,    "input"     }
};

GLShaderSource::GLShaderSource() :
    mName(""),
    mFunctionName(""),
    mReturnType(""),
    mDefinition("")
{
}

int GLShaderSource::mTimesUsed = 0;

void GLShaderSource::parseFromFile(const std::string &filename)
{
    spdlog::get("qde")->debug("ShaderSource {}: Parsing file {}", name(), filename);

    std::string shaderSource = Util::fileToString(filename);

    pugi::xml_document doc;
    doc.load_string(shaderSource.c_str());
    pugi::xml_node shaderFunction = doc.child("function");

    mFunctionName = shaderFunction.child_value("name");
    spdlog::get("qde")->debug("ShaderSource {}: Found function {}", name(), mFunctionName);

    pugi::xml_node shaderParameters = shaderFunction.child("parameters");
    parseParameters(shaderParameters);

    mDefinition = shaderFunction.child_value("source");

    spdlog::get("qde")->debug("ShaderSource {}: Parsed file {}", name(), filename);
}

void GLShaderSource::parseParameters(pugi::xml_node xmlNode)
{
    for (pugi::xml_node param : xmlNode) {
        addParameterFromXmlNode(param);
    }
}

void GLShaderSource::addParameterFromXmlNode(pugi::xml_node xmlNode)
{
    auto nameTemplate = xmlNode.child_value("name");
    auto name = fmt::format("{}_{{}}_{}", mName, nameTemplate);

    auto parameterTypeTemplate = xmlNode.child_value("type");
    auto parameterType = PARAM_STR_TO_TYPE[parameterTypeTemplate];

    // TODO: Abort if no call node is available
    auto callTemplate = xmlNode.child_value("call");
    auto call = fmt::format(callTemplate, name);

    auto builtinTemplate = xmlNode.child_value("builtin");
    auto builtin = BUILTIN_STR_TO_TYPE[builtinTemplate];

    GLShaderObjectParameter param;
    param.name = name;
    param.parameterType = parameterType;
    param.call = call;
    param.builtinType = builtin;
    mParameters.push_back(param);
    
    spdlog::get("qde")->debug("ShaderSource {}: Found parameter {}", mName, param.name);
}

NAMESPACE_END(QCE);
