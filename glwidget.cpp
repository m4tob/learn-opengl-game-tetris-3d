#include "glwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>

#include <iostream>
#include <string>
using namespace std;

// Constructor
GLWidget::GLWidget() {
    setWindowTitle("Trabalho Final - Tetris 3D");

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    cubeSize = 1; // Dimensão do bloco 1 x 1 x 1

    posX = -(boardWidth * cubeSize);
    posY = -(boardHeight * cubeSize);
    posZ = -60 * cubeSize;

    angleX = 10;
    angleY = 0;

    stepRotate = 2 * cubeSize; // Define o passo de rotação
    stepTranslate = cubeSize * 5; // Define o passo de translação
}

// Destructor
GLWidget::~GLWidget() {
    glDeleteLists(cubeListIndex, 1);
    glDeleteTextures(7, _texturesBox);
}

// Initialize OpenGL
void GLWidget::initializeGL() {
    qglClearColor(Qt::black); // Set the clear color to a black background

    glClearDepth(1); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

    setupTextures();
    setupLighting();

    drawCube();
}

// Carrega e configura as texturas que serão utilizadas no projeto
void GLWidget::setupTextures() {
    glEnable(GL_TEXTURE_2D);

    for(int x = 0; x < 7; x++) {
        string fileName = "textures/box_" + std::to_string(x);
        char _fileName[fileName.size() + 1];
        strcpy(_fileName, fileName.c_str());
        _texturesBox[x] = loadTexture(_fileName);
    }
}

// Carrega uma determinada textura a partir do nome do arquivo
GLuint GLWidget::loadTexture(QString fileName) {
    QImage img = convertToGLFormat(QImage(fileName));

    // Texture using linear filter
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    return textureId;
}

// Realiza a configuração de iluminação e sombreamento
void GLWidget::setupLighting() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambLight[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat diffLight[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightPos[] = {0.0, 5.0, 5.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

// Constrói a DisplayList responsável por gerar um dos cubos que irá compor a tela
void GLWidget::drawCube() {
    cubeListIndex = glGenLists(1);
    glNewList(cubeListIndex, GL_COMPILE);
        glBegin(GL_QUADS);
        // Front Face
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0);                 glVertex3f(-cubeSize, -cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0);          glVertex3f( cubeSize, -cubeSize, cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f( cubeSize,  cubeSize, cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0, cubeSize);          glVertex3f(-cubeSize,  cubeSize, cubeSize); // Top Left Of The Texture and Quad
        // Back Face
        glNormal3f(0, 0, -1);
        glTexCoord2f(cubeSize, 0);          glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f(-cubeSize,  cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0, cubeSize);          glVertex3f( cubeSize,  cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0, 0);                 glVertex3f( cubeSize, -cubeSize, -cubeSize); // Bottom Left Of The Texture and Quad
        // Top Face
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, cubeSize);          glVertex3f(-cubeSize, cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0, 0);                 glVertex3f(-cubeSize, cubeSize,  cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0);          glVertex3f( cubeSize, cubeSize,  cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f( cubeSize, cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        // Bottom Face
        glNormal3f(0, -1, 0);
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0, cubeSize);          glVertex3f( cubeSize, -cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0, 0);                 glVertex3f( cubeSize, -cubeSize,  cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0);          glVertex3f(-cubeSize, -cubeSize,  cubeSize); // Bottom Right Of The Texture and Quad
        // Right face
        glNormal3f(1, 0, 0);
        glTexCoord2f(cubeSize, 0);          glVertex3f( cubeSize, -cubeSize, -cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f( cubeSize,  cubeSize, -cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0, cubeSize);          glVertex3f( cubeSize,  cubeSize,  cubeSize); // Top Left Of The Texture and Quad
        glTexCoord2f(0, 0);                 glVertex3f( cubeSize, -cubeSize, cubeSize); // Bottom Left Of The Texture and Quad
        // Left Face
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, 0);                 glVertex3f(-cubeSize, -cubeSize, -cubeSize); // Bottom Left Of The Texture and Quad
        glTexCoord2f(cubeSize, 0);          glVertex3f(-cubeSize, -cubeSize,  cubeSize); // Bottom Right Of The Texture and Quad
        glTexCoord2f(cubeSize, cubeSize);   glVertex3f(-cubeSize,  cubeSize,  cubeSize); // Top Right Of The Texture and Quad
        glTexCoord2f(0, cubeSize);          glVertex3f(-cubeSize,  cubeSize, -cubeSize); // Top Left Of The Texture and Quad
        glEnd();

    glEndList();
}

// This is called when the OpenGL window is resized
void GLWidget::resizeGL(int width, int height) {
    // Prevent divide by zero (in the gluPerspective call)
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height); // Reset current viewport

    glMatrixMode(GL_PROJECTION); // Select projection matrix
    glLoadIdentity(); // Reset projection matrix

    gluPerspective(45, static_cast<GLfloat>(width)/height, 0.1, 1000); // Calculate aspect ratio

    glMatrixMode(GL_MODELVIEW); // Select modelview matrix
    glLoadIdentity(); // Reset modelview matrix
}

// OpenGL painting code goes here
void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Código de debug para preencher o board
    for(int x = 0; x < boardWidth; x++) {
        for(int y = 0; y < boardHeight; y++)  {
            board[x][y] = (x + y) % 8;
        }
    }

    glTranslatef(posX, posY, posZ);

    glRotatef(angleX, 0, 1, 0);
    glRotatef(angleY, 1, 0, 0);

    paintBoardBorder();
    paintBoardContent();

    // Framerate control
    timer->start(20);
}

void GLWidget::paintBoardBorder() {
    glBindTexture(GL_TEXTURE_2D, NULL);
    glPushMatrix();
        glTranslatef(-(6) * (cubeSize * 2), -11 * (cubeSize * 2), 0);

        glPushMatrix();
            for(int x = 0; x < (boardWidth + 2); x++) {
                glCallList(cubeListIndex);
                glTranslatef(cubeSize * 2, 0, 0);
            }
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, (boardHeight + 1) * (cubeSize * 2), 0);
            for(int x = 0; x < (boardWidth + 2); x++) {
                glCallList(cubeListIndex);
                glTranslatef(cubeSize * 2, 0, 0);
            }
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0, cubeSize * 2, 0);
            for(int y = 0; y < boardHeight; y++) {
                glCallList(cubeListIndex);
                glTranslatef(0, cubeSize * 2, 0);
            }
        glPopMatrix();

        glPushMatrix();
            glTranslatef((boardWidth+1) * (cubeSize * 2), cubeSize * 2, 0);
            for(int y = 0; y < boardHeight; y++) {
                glCallList(cubeListIndex);
                glTranslatef(0, cubeSize * 2, 0);
            }
        glPopMatrix();
    glPopMatrix();
}

void GLWidget::paintBoardContent() {
    glTranslatef(-5 * (cubeSize * 2), -10 * (cubeSize * 2), 0);

    for(int x = 0; x < boardWidth; x++) {
        glPushMatrix();
            for(int y = 0; y < boardHeight; y++)  {
                if(board[x][y] > 0) {
                    glBindTexture(GL_TEXTURE_2D, _texturesBox[board[x][y] - 1]);
                    glCallList(cubeListIndex);
                }
                glTranslatef(0, cubeSize * 2, 0);
            }
        glPopMatrix();
        glTranslatef(cubeSize * 2, 0, 0);
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;

        case Qt::Key_R:
            // Rotate
            break;

        case Qt::Key_Down:
            // Move down
            break;

        case Qt::Key_Left:
            // Move left
            break;

        case Qt::Key_Right:
            // Move Right
            break;

        case Qt::Key_8:
                posZ += stepTranslate;
            break;

        case Qt::Key_2:
                posZ -= stepTranslate;
            break;

        case Qt::Key_4:
                angleX -= stepRotate;
                angleX = angleX < 0 ? angleX + 360 : angleX;
            break;

        case Qt::Key_6:
                angleX += stepRotate;
                angleX = angleX > 360 ? angleX - 360 : angleX;
            break;

        default:
            QGLWidget::keyPressEvent(event);
    }
}

void GLWidget::changeEvent(QEvent *event) {
    switch (event->type()) {
        default:
            break;
    }
}
