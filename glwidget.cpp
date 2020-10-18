#include "glwidget.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QTimer>

// Constructor
GLWidget::GLWidget() {
    setWindowTitle("Trabalho Final - Tetris 3D");

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    cubeSize = 1; // Dimensão do bloco 1 x 1 x 1
}

// Destructor
GLWidget::~GLWidget() {
    glDeleteLists(cubeListIndex, 1);
    glDeleteTextures(1, &_textureBox);
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
    _textureBox = loadTexture("textures/box.png");
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

        glBindTexture(GL_TEXTURE_2D, _textureBox);
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

    // Framerate control
    timer->start(20);
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
            // Rotate perspective (top)
            break;

        case Qt::Key_2:
            // Rotate perspective (down)
            break;

        case Qt::Key_4:
            // Rotate perspective (left)
            break;

        case Qt::Key_6:
            // Rotate perspective (right)
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
