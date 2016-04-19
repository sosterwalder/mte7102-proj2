#include "glshader.hpp"
#include "glshaderparameter.hpp"


NAMESPACE_BEGIN(QCE);

GLShaderFloatParameter::GLShaderFloatParameter() :
    GLShaderParameter(),
    mData(0.0f) {}

void GLShaderFloatParameter::setUniform(OpenGLShader *shader)
{
    shader->setUniform(mName, mData);
}

GLShaderVector3fParameter::GLShaderVector3fParameter() :
    mData(Eigen::Vector3f(0.0f, 0.0f, 0.0f)) {}

void GLShaderVector3fParameter::setUniform(OpenGLShader *shader)
{
    shader->setUniform(mName, mData);
}

NAMESPACE_END(QCE);