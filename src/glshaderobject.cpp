#include <src/pugiconfig.hpp>
#include "common.hpp"
#include "util.hpp"
#include "glshaderobject.hpp"


NAMESPACE_BEGIN(QCE);

std::map<std::string, BuiltinType> GLShaderObject::BUILTIN_STR_TO_TYPE = {
    { "unknown", BuiltinType::UNKNOWN},
    { "float",   BuiltinType::FLOAT  },
    { "vec3",    BuiltinType::VEC3   }
};
std::map<BuiltinType, std::string> GLShaderObject::TYPE_TO_BUILTIN_STR = {
    { BuiltinType::UNKNOWN, "unknown"},
    { BuiltinType::FLOAT,   "float"  },
    { BuiltinType::VEC3,    "vec3"   }
};
std::map<std::string, ParameterType> GLShaderObject::PARAM_STR_TO_TYPE = {
    { "unknown",  ParameterType::UNKNOWN },
    { "property", ParameterType::PROPERTY},
    { "sink",     ParameterType::SINK    }
};
std::map<ParameterType, std::string> GLShaderObject::PARAM_TYPE_TO_STR = {
    { ParameterType::UNKNOWN,  "unknown"  },
    { ParameterType::PROPERTY, "property" },
    { ParameterType::SINK,     "sink"     }
};
int GLShaderObject::mTimesUsed = 0;

GLShaderObject::GLShaderObject() :
    mId(mTimesUsed),
    mName("ShaderObject"),
    mReturnType(""),
    mDefinition("")
{
    spdlog::get("qde")->debug("ShaderObject {}: Getting constructed", identifier(), (void*)this);
}

std::string GLShaderObject::uniforms()
{
    fmt::MemoryWriter out;

    /*
    TODO: Fix this
    size_t remainingParamters = mParameters.size();
    for (auto i = mParameters.begin(); i != mParameters.end(); ++i) {
        auto parameter = *i;
        auto name = parameter.name();

        out << "uniform ";
        out << TYPE_TO_BUILTIN_STR[parameter.builtinType()];
        out << " ";
        out << name;
        out << ";";

        // If we are not at the last parameter, add a new line
        if (remainingParamters-- != 1) {
            out << "\n";
        }
    }
    */

    return out.str();
}

std::string GLShaderObject::call()
{
    fmt::MemoryWriter out;
    /*
    TODO: Fix this
    size_t remainingParamters = mParameters.size();

    out << mFunctionName << "(";

    for (auto i = mParameters.begin(); i != mParameters.end(); ++i)
    {
        auto parameter = *i;
        auto name = parameter.name();
        auto call = fmt::format(parameter.call(), name);

        out << call;

        // If we are not at the last parameter add a comma for separatation
        if (remainingParamters-- != 1) {
            out << ", ";
        }
    }
    out << ");";
    */

    spdlog::get("qde")->debug("ShaderObject {}: Calculated output", name());

    return out.str();
}

void GLShaderObject::parseFromFile(const std::string &filename)
{
    spdlog::get("qde")->debug("ShaderObject {}: Parsing file {}", name(), filename);

    std::string shaderSource = Util::fileToString(filename);

    pugi::xml_document doc;
    doc.load_string(shaderSource.c_str());
    pugi::xml_node shaderFunction = doc.child("function");

    mFunctionName = shaderFunction.child_value("name");
    spdlog::get("qde")->debug("ShaderObject {}: Found function {}", name(), mFunctionName);

    pugi::xml_node shaderParameters = shaderFunction.child("parameters");
    parseParameters(shaderParameters);

    mDefinition = shaderFunction.child_value("source");

    spdlog::get("qde")->debug("ShaderObject {}: Parsed file {}", name(), filename);
}

void GLShaderObject::parseParameters(pugi::xml_node xmlNode)
{
    for (pugi::xml_node param : xmlNode) {
        addParameterFromXmlNode(param);
    }
}

void GLShaderObject::addParameterFromXmlNode(pugi::xml_node xmlNode)
{
    auto nameTemplate = xmlNode.child_value("name");
    auto name = fmt::format("{}_{}_{}", mName, mTimesUsed, nameTemplate);
    auto callTemplate = xmlNode.child_value("call");
    auto call = fmt::format(callTemplate, name);
    auto builtinTemplate = xmlNode.child_value("builtin");
    auto builtin = BUILTIN_STR_TO_TYPE[builtinTemplate];
    auto parameterTypeTemplate = xmlNode.child_value("type");
    auto parameterType = PARAM_STR_TO_TYPE[parameterTypeTemplate];

    spdlog::get("qde")->debug("ShaderObject {}: Found parameter {}", id(), name);

    switch (builtin) {
        case BuiltinType::FLOAT:
            addFloatParameter(name, call, builtin, parameterType);
            break;

        case BuiltinType::VEC3:
            addVector3fParameter(name, call, builtin, parameterType);
            break;

        default:
            throw new std::runtime_error("Unknown builtin type");
            break;
    }

    spdlog::get("qde")->debug("ShaderObject {}: Added parameter: {}", id(), name);
}

void GLShaderObject::addFloatParameter(const std::string &name, const std::string &call, BuiltinType builtinType, ParameterType parameterType)
{
    
    GLShaderFloatParameter *floatParam = new GLShaderFloatParameter();
    floatParam->setName(name);
    floatParam->setCall(call);
    floatParam->setBuiltinType(builtinType);
    floatParam->setParameterType(parameterType);
    
    mParameters.push_back(std::unique_ptr<GLShaderFloatParameter>(floatParam));
    
    delete floatParam;
}

void GLShaderObject::addVector3fParameter(const std::string &name, const std::string &call, BuiltinType builtinType, ParameterType parameterType)
{
    GLShaderVector3fParameter *vec3fParam = new GLShaderVector3fParameter();
    vec3fParam->setName(name);
    vec3fParam->setCall(call);
    vec3fParam->setBuiltinType(builtinType);
    vec3fParam->setParameterType(parameterType);
    
    mParameters.push_back(std::unique_ptr<GLShaderVector3fParameter>(vec3fParam));
    
    delete vec3fParam;
}

NAMESPACE_END(QCE);