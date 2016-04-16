#pragma once

#include "main.hpp"


class GLShaderObject : public nanogui::Object
{
public:
    GLShaderObject();
    const int &identifier() { return mId; }
    void setName(const std::string &name) { mName = name; }
    const std::string &name() const { return mName; }
    void setFunctionName(const std::string &name) { mFunctionName = name; }
    const std::string &functionName() const { return mFunctionName; }
    std::string id() const { return fmt::format("{}-{}", mName, mId); }
    void setReturnRype(const std::string &returnType) { mReturnType = returnType; }
    const std::string &definition() const { return mDefinition; }
    std::string uniforms();
    std::string call();
    void parseFromFile(const std::string &filename);
    void incTimesUsed() { mId = mTimesUsed; mTimesUsed++; }
    void decTimesUsed() { mTimesUsed--; }
    int timesUsed() { return mTimesUsed; }
    std::vector<GLShaderParameter> parameters() { return mParameters; }
    //const std::vector<GLShaderParameter> &parameters { return mParameters; }

protected:
    static std::map<std::string, BuiltinType> BUILTIN_STR_TO_TYPE;
    static std::map<BuiltinType, std::string> TYPE_TO_BUILTIN_STR;
    static std::map<std::string, ParameterType> PARAM_STR_TO_TYPE;
    static std::map<ParameterType, std::string> PARAM_TYPE_TO_STR;

    int mId;
    std::string mName;
    std::string mFunctionName;
    std::string mReturnType;
    std::string mDefinition;
    std::vector<GLShaderParameter> mParameters;

private:
    static int mTimesUsed;

    void parseParameters(pugi::xml_node xmlNode);
    void addParameterFromXmlNode(pugi::xml_node xmlNode);
};
