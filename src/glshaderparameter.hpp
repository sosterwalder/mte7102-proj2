#pragma once

#include "main.hpp"

/*
class GLShaderParameterInterface : public nanogui::Object
{
public:
    virtual ~GLShaderParameterInterface() {}
    virtual void *data() { return 0; }
    // virtual void *type() { return 0; }
    virtual GLShaderParameterInterface *clone() const { return 0; }
};

template<class T>
class GLShaderParameterImpl : public GLShaderParameterInterface
{
public:
    GLShaderParameterImpl(const T &v) :
        mData(v) {}
    virtual ~GLShaderParameterImpl() {}
    virtual void *data() { return &mData; }
    // virtual void *type() { return &mType; }
    virtual GLShaderParameterImpl *clone() const { return new GLShaderParameterImpl<T>(mData); }

    // static void * staticType() { return &mType; }

protected:
    T mData;
    // static int mType;

};

class GLShaderParameterData : public nanogui::Object
{
public:
    template<class T>
    GLShaderParameterData(const T &v) { mParameter = new GLShaderParameterImpl<T>(v); }
    GLShaderParameterData(const GLShaderParameterData &o) { mParameter = o.mParameter->clone(); }
    ~GLShaderParameterData() { delete mParameter; }

protected:
    GLShaderParameterInterface *mParameter;

    GLShaderParameterData() {}
};


class GLShaderParameterInterface : public nanogui::Object
{
public:
    virtual ~GLShaderParameterInterface() {}
    virtual std::unique_ptr<GLShaderParameterInterface> data() const = 0;
};

template<typename T>
class GLShaderParameterImpl : public GLShaderParameterInterface
{
public:
    template<typename ...Ts>
    GLShaderParameterImpl(Ts&&...ts) :
        t(std::forward<Ts>(ts)...) {}
    virtual std::unique_ptr<GLShaderParameterInterface> data() const override { return std::unique_ptr<GLShaderParameterImpl<T>>(new GLShaderParameterImpl<T>(t)); }

private:
    T t;
};

class GLShaderParameter : public nanogui::Object
{
public:
    template<typename T>
    GLShaderParameter(T t) :
        mData(std::unique_ptr<GLShaderParameterImpl<T>>(std::move(t))) {}

private:
    std::unique_ptr<GLShaderParameterInterface> mData;
};
*/
enum class BuiltinType
{
    UNKNOWN,
    FLOAT,
    VEC3
};

enum class ParameterType
{
    UNKNOWN,
    PROPERTY,
    SINK
};

typedef boost::variant<Eigen::Vector3f, float> paramVariant;

class GLShaderParameter : public nanogui::Object
{
public:
    GLShaderParameter() {}
    GLShaderParameter(const std::string &name, const std::string &call, paramVariant data) :
        mName(name),
        mCall(call),
        mData(data),
        mBuiltinType(BuiltinType::UNKNOWN),
        mParameterType(ParameterType::UNKNOWN) {}
    GLShaderParameter(const std::string &name, const std::string &call, paramVariant data, BuiltinType builtinType, ParameterType parameterType) :
        mName(name),
        mCall(call),
        mData(data),
        mBuiltinType(builtinType),
        mParameterType(parameterType) {}

    void setName(const std::string &name) { mName = name; }
    const std::string &name() const { return mName; }
    void setCall(const std::string &call) { mCall = call; }
    const std::string &call() const { return mCall; }
    void setData(paramVariant data) { mData = data; }
    paramVariant data() { return mData; }
    void setBuiltinType(BuiltinType builtinType) { mBuiltinType = builtinType; }
    BuiltinType builtinType() { return mBuiltinType; }
    void setParameterType(ParameterType parameterType) { mParameterType = parameterType; }
    ParameterType parameterType() { return mParameterType; }

protected:
    std::string mName;
    std::string mCall;
    paramVariant mData;
    BuiltinType mBuiltinType;
    ParameterType mParameterType;
};
