#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include "common.hpp"
#include "graph.hpp"
#include "graphnode.hpp"
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
    mNumFrames(0)
{
    nanogui::Window *toolsWindow = new nanogui::Window(this, "Tools");
    toolsWindow->setId("toolsWindow");
    toolsWindow->setPosition(Eigen::Vector2i(10, 10));
    toolsWindow->setWidth(200);
    toolsWindow->setHeight(768-10-10);
    spdlog::get("qde")->debug("Created tools window");

    mNodeGraph = new Graph(this, this, "Nodes");
    mNodeGraph->setId("nodeGraph");
    mNodeGraph->setParent(this);
    mNodeGraph->setPosition(Eigen::Vector2i(220, 50 /*768-210*/));
    mNodeGraph->setWidth(1024-220-10);
    mNodeGraph->setHeight(200);
    spdlog::get("qde")->debug("Created nodes window");

    initializeShaderObjects();
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

    mNodeGraph->drawContents();

    // Draw the window contents using OpenGL
    mShader.bind();

    Eigen::Matrix4f mvp;
    mvp.setIdentity();
    mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(
        Eigen::AngleAxisf(
            (float) glfwGetTime(),
            Eigen::Vector3f::UnitZ()
        )
    ) * 0.25f;

    mvp.row(0) *= (float) mSize.y() / (float) mSize.x();
    // mShader.setUniform("modelViewProj", mvp);

    /* Draw 2 triangles starting at index 0 */
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);

    // Update drawing time
    if (currentTime - mUpdateTime >= 1.0) {
        double msPerFrame = 1000.0 / (double)mNumFrames;

        std::stringstream strOut;
        setCaption(fmt::format(
            "QCE ({} frames, {} ms/frame)",
            mNumFrames, msPerFrame
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
    mShader.addShaderObjectToOutput(shaderObject->name());
    mShader.setUniforms();
    mShader.recompile();
}

void Qce::setShaderOutput(const std::string &output)
{
    mShader.setFragmentShaderCalls(output);
    mShader.recompile();
}

// Loading files
void Qce::initializeShaderObjects()
{
    std::string path = "data/objects";

    for (auto foundObject : Util::findShaderObjects(path)) {
        spdlog::get("qde")->debug(
            "Found and adding shader object node: {} - {}",
            foundObject.first,
            foundObject.second
        );
        GLShaderObject *shaderObject = new GLShaderObject();
        shaderObject->setName(foundObject.first);
        shaderObject->parseFromFile(foundObject.second);
        spdlog::get("qde")->debug(
            "ShaderObject: {}",
            shaderObject->getRefCount()
        );
        mNodeGraph->addNodeType(shaderObject);
        mShader.addShaderObject(shaderObject);
    }
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
        "uniform mat4 modelViewProj;\n"
        "in vec3 position;\n"
        "void main() {\n"
        "    gl_Position = vec4(position, 1.0);\n"
        "}"
    );
    mShader.recompile();
}

NAMESPACE_END(QCE);