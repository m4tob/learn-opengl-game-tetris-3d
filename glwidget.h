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
    void paintSky();
    void drawGrass();
    void drawCube();
    void drawPyramid();

    GLuint _textureGrass, _textureSand;
    GLuint grassListIndex, cubeListIndex, pyramidListIndex;
    QTimer *timer;

    GLint pyramidHeight;
    GLfloat cubeSize;

    GLchar keyMode;
    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, angleY, posX, posY, posZ;
};

#endif // GLWIDGET_H
