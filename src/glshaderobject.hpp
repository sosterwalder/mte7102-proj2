#pragma once

#include "main.hpp"

enum class GLShaderBuiltinType
{
    UNKNOWN,
    FLOAT,
    VEC3
};

enum class GLShaderType
{
    UNKNOWN,
    PROPERTY,
    SINK
};

struct GLShaderBuiltin
{
    std::string name;
    GLShaderBuiltinType builtinType;
    GLShaderType type;
};

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

protected:
    static std::map<std::string, GLShaderBuiltinType> BUILTIN_STR_TO_TYPE;
    static std::map<GLShaderBuiltinType, std::string> TYPE_TO_BUILTIN_STR;
    static std::map<std::string, GLShaderType> STR_TO_TYPE;
    static std::map<GLShaderType, std::string> TYPE_TO_STR;

    int mId;
    std::string mName;
    std::string mFunctionName;
    std::string mReturnType;
    std::string mDefinition;
    TypedProperties<GLShaderBuiltin> mParameters;

    void addParameterFromBuiltin(const GLShaderBuiltin &builtin);

private:
    static int mTimesUsed;
};

