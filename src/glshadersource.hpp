#pragma once

#include <src/pugixml.hpp>
#include <nanogui/object.h>
#include "glshaderparameter.hpp"


NAMESPACE_BEGIN(QCE);

struct GLShaderObjectParameter
{
    std::string name;
    ParameterType parameterType;
    std::string call;
    BuiltinType builtinType;
};
typedef GLShaderObjectParameter GLShaderObjectParameter;

class GLShaderSource : public nanogui::Object
{
public:
    GLShaderSource();
    void setName(const std::string &name) { mName = name; }
    const std::string &name() const { return mName; }
    std::string nameTemplate() const { return fmt::format("{}-{{}}", mName); }
    void setFunctionName(const std::string &name) { mFunctionName = name; }
    const std::string &functionName() const { return mFunctionName; }
    void setReturnRype(const std::string &returnType) { mReturnType = returnType; }
    const std::string &definition() const { return mDefinition; }
    void parseFromFile(const std::string &filename);
    const std::vector<GLShaderObjectParameter> &parameters() { return mParameters; }
    void incTimesUsed() { mTimesUsed++; }
    void decTimesUsed() { mTimesUsed--; }
    int timesUsed() { return mTimesUsed; }
    static const BuiltinType &builtinType(const std::string &builtinName) { return BUILTIN_STR_TO_TYPE[builtinName]; }
    static const std::string &builtinString(const BuiltinType &builtinType) { return TYPE_TO_BUILTIN_STR[builtinType]; }

protected:
    static std::map<std::string, BuiltinType> BUILTIN_STR_TO_TYPE;
    static std::map<BuiltinType, std::string> TYPE_TO_BUILTIN_STR;
    static std::map<std::string, ParameterType> PARAM_STR_TO_TYPE;
    static std::map<ParameterType, std::string> PARAM_TYPE_TO_STR;

    std::string mName;
    std::string mFunctionName;
    std::string mReturnType;
    std::string mDefinition;
    std::vector<GLShaderObjectParameter> mParameters;

private:
    static int mTimesUsed;
    
    void parseParameters(pugi::xml_node xmlNode);
    void addParameterFromXmlNode(pugi::xml_node xmlNode);
    void addProperty(const GLShaderObjectParameter &parameter);
    void addInput(const GLShaderObjectParameter &parameter);
};

NAMESPACE_END(QCE);
