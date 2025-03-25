#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    SceneWidget(QWidget* parent = nullptr);
    ~SceneWidget();
    void mouseReleaseEvent(QMouseEvent* event) override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* shaderProgram;
    QOpenGLBuffer vertexBuffer;
};

#endif  // SCENEWIDGET_H
