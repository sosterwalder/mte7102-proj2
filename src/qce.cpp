#include "main.hpp"

using namespace nanogui;

Qce::Qce() :
    Screen(Eigen::Vector2i(1024, 768), "QCE"),
    mIntensity(1.0f)
{
    Window *toolsWindow = new Window(this, "Tools");
    toolsWindow->setPosition(Vector2i(10, 10));
    toolsWindow->setWidth(200);
    toolsWindow->setHeight(768-10-10);
    spdlog::get("qde")->debug("Created tools window");

    Graph *nodeGraph = new Graph(this, "Nodes");
    nodeGraph->setParent(this);
    nodeGraph->setPosition(Vector2i(220, 768-210));
    nodeGraph->setWidth(1024-220-10);
    nodeGraph->setHeight(200);
    spdlog::get("qde")->debug("Created nodes window");

    performLayout(mNVGContext);

    mShader.init(
        /* An identifying name */
        "a_simple_shader",

        /* Vertex shader */
        "#version 330\n"
        "uniform mat4 modelViewProj;\n"
        "in vec3 position;\n"
        "void main() {\n"
        "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
        "}",

        /* Fragment shader */
        "#version 330\n"
        "out vec4 color;\n"
        "uniform float intensity;\n"
        "void main() {\n"
        "    color = vec4(vec3(intensity), 1.0);\n"
        "}"
    );
    bindShader();
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
    /* Draw the window contents using OpenGL */
    mShader.bind();

    Matrix4f mvp;
    mvp.setIdentity();
    mvp.topLeftCorner<3,3>() = Matrix3f(
        Eigen::AngleAxisf((float) glfwGetTime(),  Vector3f::UnitZ())
    ) * 0.25f;

    mvp.row(0) *= (float) mSize.y() / (float) mSize.x();

    mShader.setUniform("modelViewProj", mvp);

    /* Draw 2 triangles starting at index 0 */
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
}

bool Qce::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
        return true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return true;
    }
    return false;
}

void Qce::bindShader()
{
    spdlog::get("qde")->debug("Binding shader");

    MatrixXu indices(3, 2); /* Draw 2 triangles */
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    MatrixXf positions(3, 4);
    positions.col(0) << -1, -1, 0;
    positions.col(1) <<  1, -1, 0;
    positions.col(2) <<  1,  1, 0;
    positions.col(3) << -1,  1, 0;

    mShader.bind();
    mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
    mShader.setUniform("intensity", mIntensity);
}
