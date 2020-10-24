#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QHash>

#include "board.h"

class QTimer;

class GLWidget : public QGLWidget {
    Q_OBJECT

public:
    GLWidget();
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int boardWidth, int boardHeight);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

public Q_SLOTS:
    virtual void updateGame();

private:
    Board board;

    void setupTextures();
    GLuint loadTexture(QString image);
    void setupLighting();
    void drawCube();
    void paintBoardBorder();
    void paintBoardContent();

    GLuint _textureBorder;
    GLuint *_texturesBox; // Uma para cada um dos tipos de peça

    GLuint cubeListIndex, viewListIndex;
    QTimer *timer, *gameTimer;
    GLint animationTime;

    GLfloat cubeSize;

    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, angleY, posX, posY, posZ;
};

#endif // GLWIDGET_H
