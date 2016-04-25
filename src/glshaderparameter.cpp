#include "common.hpp"
#include "glshaderobject.hpp"
#include "glshader.hpp"
#include "glshaderparameter.hpp"


NAMESPACE_BEGIN(QCE);

std::string GLShaderInputParameter::call()
{
    fmt::MemoryWriter out;
    
    if (mInput.get() != nullptr) {
        out << mInput->call();
    }
    
    return out.str();
}

GLShaderFloatParameter::GLShaderFloatParameter() :
    GLShaderParameter(),
    mData(0.0f) {}

std::string GLShaderFloatParameter::call()
{
    return mCall;
}

void GLShaderFloatParameter::setUniform(OpenGLShader *shader)
{
    shader->setUniform(mName, mData);
}

GLShaderVector3fParameter::GLShaderVector3fParameter() :
    mData(Eigen::Vector3f(0.0f, 0.0f, 0.0f)) {}

std::string GLShaderVector3fParameter::call()
{
    return mCall;
}

void GLShaderVector3fParameter::setUniform(OpenGLShader *shader)
{
    shader->setUniform(mName, mData);
}

NAMESPACE_END(QCE);