#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include "common.hpp"
#include "camera.hpp"
#include "graph.hpp"
#include "graphnode.hpp"
#include "glshadersource.hpp"
#include "glshaderobject.hpp"
#include "util.hpp"
#include "qce.hpp"


NAMESPACE_BEGIN(QCE);

Qce::Qce() :
    Screen(Eigen::Vector2i(1024, 768), "QCE"),
    mNodeGraph(nullptr),
    mIntensity(1.0f),
    mStartTime(glfwGetTime()),
    mUpdateTime(glfwGetTime()),
    mNumFrames(0),
    mCamZ(0.0f)
{
    mNodeGraph = new Graph(this, this, "Nodes");
    mNodeGraph->setId("nodeGraph");
    mNodeGraph->setParent(this);
    mNodeGraph->setPosition(Eigen::Vector2i(220, 768-210));
    mNodeGraph->setWidth(1024-220-10);
    mNodeGraph->setHeight(200);
    spdlog::get("qde")->debug("Created nodes window");

    initializeShaderFiles();
    initializeShader();

    performLayout(mNVGContext);
}

Qce::~Qce()
{
    mShader.free();
}

void Qce::draw(NVGcontext *ctx)
{
    Screen::draw(ctx);
}

void Qce::drawContents()
{
    double currentTime = glfwGetTime();
    mNumFrames++;

    // Draw the window contents using OpenGL
    bindShader();

    Eigen::Matrix4f projectionMatrix = Camera::perspective<Eigen::Matrix4f::Scalar>(50, mSize.x()/mSize.y(), 0.01f, 10.0f);
    Eigen::Matrix4f viewMatrix       = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f modelMatrix      = Eigen::Matrix4f::Identity();

    Eigen::Affine3f viewTransform(Eigen::Translation3f(0, 0, mCamZ));
    viewMatrix = viewTransform.matrix();

    // Eigen::Affine3f modelTransform(Eigen::Translation3f(-.5f, -.5f, -.5f));
    // modelMatrix = modelTransform.matrix();

    Eigen::Matrix4f mvp(projectionMatrix);
    Eigen::Matrix4f mv(viewMatrix);

    mv  *= modelMatrix;
    mvp *= mv;


    // Eigen::Matrix4f mvp;
    // mvp.setIdentity();
    // mvp.row(0) *= (float) mSize.y() / (float) mSize.x();
    mShader.setUniform("modelViewProj", mvp);

    /*
    mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(
        Eigen::AngleAxisf(
            (float) glfwGetTime(),
            Eigen::Vector3f::UnitZ()
        )
    ) * 0.25f;
    */


    /* Draw 2 triangles starting at index 0 */
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);

    // Update drawing time
    if (currentTime - mUpdateTime >= 1.0) {
        double msPerFrame = 1000.0 / (double)mNumFrames;

        std::stringstream strOut;
        setCaption(fmt::format(
            "QCE ({} frames, {} ms/frame, {} camera Z)",
            mNumFrames, msPerFrame, mCamZ
        ));

        mNumFrames   = 0;
        mUpdateTime += 1.0;
    }
}

bool Qce::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers)) {
        return true;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);

        return true;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        mCamZ += 0.5f;

        return true;
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mCamZ -= 0.5f;

        return true;
    }

    return false;
}

void Qce::bindShader()
{
    // spdlog::get("qde")->debug("Binding shader");

    nanogui::MatrixXu indices(3, 2); /* Draw 2 triangles */
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    nanogui::MatrixXf positions(3, 4);
    positions.col(0) << -1, -1, 0;
    positions.col(1) <<  1, -1, 0;
    positions.col(2) <<  1,  1, 0;
    positions.col(3) << -1,  1, 0;

    mShader.bind();
    mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
    mShader.setUniforms();

    mShader.setUniform("globalShowDistance", 1);
    // spdlog::get("qde")->debug("QCE: Shader output: {}", mShader.fragmentShaderSource());
}

// When connecting node
void Qce::addShaderToOutput(GLShaderObject *shaderObject)
{
    spdlog::get("qde")->debug(
        "QCE: Adding shader object to output: {}",
        shaderObject->name()
    );
    // This is needed for binding the uniforms later on
    mShader.addShaderObject(shaderObject);
    mShader.setUniforms();
    mShader.recompile();
}

void Qce::setShaderCamera(const std::string &camera)
{
    mShader.setFragmentShaderCamera(camera);
    mShader.recompile();
}

void Qce::setShaderOutput(const std::string &output)
{
    mShader.setFragmentShaderCalls(output);
    mShader.recompile();
}

void Qce::findAndAddShaderFiles(const std::string &path)
{
    for (auto foundObject : Util::findShaderFiles(path)) {
        spdlog::get("qde")->debug(
            "Found and added shader file: {} - {}",
            foundObject.first,
            foundObject.second
        );
        nanogui::ref<GLShaderSource> shaderSource = new GLShaderSource();
        shaderSource->setName(foundObject.first);
        shaderSource->parseFromFile(foundObject.second);
        mShaderSources.push_back(shaderSource);
        mNodeGraph->addNodeType(shaderSource);
    }
}

// Loading files
void Qce::initializeShaderFiles()
{
    spdlog::get("qde")->debug("QCE: Searching for shader files");
    findAndAddShaderFiles("data/misc/");
    findAndAddShaderFiles("data/operations/");
    findAndAddShaderFiles("data/objects/");
}

void Qce::initializeShader()
{
    std::string filePath = "data/sphere_tracer.fs";
    std::string fragmentShaderSource = Util::fileToString(filePath);
    mShader.setName("sphere_tracer");
    mShader.setFragmentShaderTemplate(fragmentShaderSource);
    mShader.setVertexShaderSource(
        /* Vertex shader */
        "#version 330\n"
        "in vec3 position;\n"
        "void main() {\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}"
    );
    mShader.recompile();
}

NAMESPACE_END(QCE);
