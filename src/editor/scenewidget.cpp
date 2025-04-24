#include "scenewidget.h"

namespace {
const int kMajor = 3;
const int kMinor = 3;

const float kGlColor = 0.1f;

// vertexes of the triangle
const GLfloat kVertices[] = {
    -0.5f, -0.5f, 0.0f, // left
    0.5f,  -0.5f, 0.0f, // right
    0.0f,  0.5f,  0.0f // top
};
} // namespace

SceneWidget::SceneWidget(QWidget *parent) : QOpenGLWidget(parent), m_shaderProgram(nullptr)
{
    // enable OpenGL support
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(kMajor, kMinor);
    setFormat(format);
}

SceneWidget::~SceneWidget()
{
    m_vertexBuffer.destroy();
    delete m_shaderProgram;
}

void SceneWidget::mouseReleaseEvent(QMouseEvent *)
{
    qDebug() << mapFromGlobal(QCursor::pos());
}

void SceneWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // set bg color
    glClearColor(kGlColor, kGlColor, kGlColor, 1.0f);

    // create shader program
    m_shaderProgram = new QOpenGLShaderProgram(this);
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "#version 330 core\n"
                                           "layout(location = 0) in vec3 position;\n"
                                           "void main()\n"
                                           "{\n"
                                           "    gl_Position = vec4(position, 1.0);\n"
                                           "}\n");
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "#version 330 core\n"
                                           "out vec4 fragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "    fragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                                           "}\n");
    m_shaderProgram->link();

    // create and set up vertex buffer
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(kVertices, sizeof(kVertices));
}
void SceneWidget::resizeGL(int w, int h)
{
    // rendering area
    glViewport(0, 0, w, h);
}

void SceneWidget::paintGL()
{
    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shaderProgram->bind();
    m_vertexBuffer.bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // disable shader program
    m_shaderProgram->release();
}
