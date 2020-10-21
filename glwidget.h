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
    void resizeGL(int boardWidth, int boardHeight);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void changeEvent(QEvent *event);

private:
    void setupTextures();
    GLuint loadTexture(QString image);
    void setupLighting();
    void drawCube();
    void paintBoardBorder();
    void paintBoardContent();

    enum Piece { ZRight, ZLeft, Line, T, Square, LRight, LLeft };
    GLuint _texturesBox[7]; // Uma para cada um dos tipos de peça

    int boardWidth = 10, boardHeight = 20;
    int board[10][20];

    GLuint cubeListIndex, viewListIndex;
    QTimer *timer;

    GLfloat cubeSize;

    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, angleY, posX, posY, posZ;
};

#endif // GLWIDGET_H
