#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QHash>

class QTimer;

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget();
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

private:
    void setupTextures();
    GLuint loadTexture(QString image);
    void setupLighting();
    void drawCube();

    GLuint _textureBox;

    GLuint cubeListIndex, viewListIndex;
    QTimer *timer;

    GLfloat cubeSize;
};

#endif // GLWIDGET_H
