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

    pyramidHeight = 45; // Altura da pirâmide em quantidade de blocos
    cubeSize = 1; // Dimensão do bloco 1 x 1 x 1

    angleX = 20; // Ângulo inicial no eixo X
    angleY = 0; // Ângulo inicial no eixo Y
    posX = 0; // Posição inicial no eixo X
    posY = -pyramidHeight/2; // Posição inicial no eixo Y (A o Centro da pirâmide é posicionado no centro da tela)
    posZ = -6 * pyramidHeight  * cubeSize; // Posição inicial no eixo Z (Afasta a pirâmide permitindo visualizá-la completamente)

    keyMode = 'R'; // Inicia a interação no modo de rotação
    stepRotate = 2 * cubeSize; // Define o passo de rotação
    stepTranslate = cubeSize * 5; // Define o passo de translação
}

// Destructor
GLWidget::~GLWidget() {
    glDeleteLists(cubeListIndex, 1);
    glDeleteTextures(1, &_textureSand);
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

    drawGrass();
    drawCube();
    drawPyramid();
}

// Carrega e configura as texturas que serão utilizadas no projeto
void GLWidget::setupTextures() {
    glEnable(GL_TEXTURE_2D);
    _textureSand = loadTexture("textures/sand.jpg");
    _textureGrass = loadTexture("textures/grass.jpg");
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

// Constrói a DisplayList responsável por gerar a grama
void GLWidget::drawGrass() {
    grassListIndex = glGenLists(1);
    glNewList(grassListIndex, GL_COMPILE);

        // Define a largura (e comprimento) da grama como sendo 20 vezes a largura da pirâmide
        GLfloat width = 20 * (pyramidHeight * cubeSize);

        glBindTexture(GL_TEXTURE_2D, _textureGrass);
        glBegin(GL_QUADS);
            glTexCoord2f(width, width); glVertex3f(-width, cubeSize - 0.01, -width);
            glTexCoord2f(0, width);     glVertex3f( width, cubeSize - 0.01, -width);
            glTexCoord2f(0, 0);         glVertex3f( width, cubeSize - 0.01,  width);
            glTexCoord2f(width, 0);     glVertex3f(-width, cubeSize - 0.01,  width);
        glEnd();

    glEndList();
}

// Constrói a DisplayList responsável por gerar um dos cubos que irá compor a pirâmide
void GLWidget::drawCube() {
    cubeListIndex = glGenLists(1);
    glNewList(cubeListIndex, GL_COMPILE);

        glBindTexture(GL_TEXTURE_2D, _textureSand);
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

// Constrói a DisplayList responsável por gerar a pirâmide.
// A pirâmide é construída de cima para baixo (começando com o cubo do topo)
// e da esquerda (-X) para a direita (+X) no eixo X e da frente (-Z) para o fundo (+Z)
// do eixo Z.
void GLWidget::drawPyramid() {
    pyramidListIndex = glGenLists(1);
    glNewList(pyramidListIndex, GL_COMPILE);
        GLint size; // Número de cubos em uma linha (1 cubo na primeira, 3 na segunda, 5 na terceira, etc)
        GLint offset = 2 * cubeSize; // Offset entre um cubo e outro.

        // A primeira linha é considerada o topo da pirâmide
        for(GLint row = 0; row < pyramidHeight; row++) {
            size = 1 + (row * 2); // cubo central, mais 2*row, sendo cada 1 * row para um dos lados do cubo central

            glPushMatrix(); // Adiciona a matriz na pilha para não precisar manipular os valores de volta.
                glTranslatef(0, (pyramidHeight - row) * offset, 0); // Desce uma linha da pirâmide
                glTranslatef(-(row * offset), 0, 0); // Posiciona a origem no ponto mais extremo de X

                for(GLint x = 0; x < size; x++) {
                    glPushMatrix(); // Armazena a matriz com a configuração atual (X e Y configurados)
                        glTranslatef(x * offset, 0, 0); // Move a origem para o próximo ponto de X
                        glTranslatef(0, 0, -(row * offset)); // Posiciona a origem no ponto mais extremo de Z

                        for(GLint z = 0; z < size; z++) {
                            glTranslatef(0, 0, offset);  // Move a origem para o próximo ponto de Z
                            glCallList(cubeListIndex); // Dsenha um cubo a partir da DisplayList
                        }
                    glPopMatrix(); // Após preencher toda a linha do eixo Z, volta para o início com X e Y configurados
                }
            glPopMatrix(); // Volta para a matriz na situação do início da função
        }

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

    glTranslatef(posX, posY, posZ); // Posiciona a pirâmide a partir dos parâmetros de interação
    glRotatef(angleX, 0, 1, 0); // Realiza a rotação no eixo X, a partir do parâmetro de interação
    glRotatef(angleY, 1, 0, 0); // Realiza a rotação no eixo Y, a partir do parâmetro de interação

    glCallList(grassListIndex); // Dsenha a grama a partir de sua DisplayList
    glCallList(pyramidListIndex); // Dsenha a pirâmide a partir de sua DisplayList

    // Framerate control
    timer->start(20);
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;

        case Qt::Key_R: // Altera o modo para rotação
            keyMode = 'R';
            break;

        case Qt::Key_T: // Altera o modo para translação
            keyMode = 'T';
            break;

        case Qt::Key_Up:
            if(keyMode == 'R') { // Rotação
                angleY += stepRotate;
                angleY = angleY > 360 ? angleY - 360 : angleY;
            } else if(keyMode == 'T') { // Translação
                posZ += stepTranslate;
            }
            break;

        case Qt::Key_Down:
            if(keyMode == 'R') { // Rotação
                angleY -= stepRotate;
                angleY = angleY < 0 ? angleY + 360 : angleY;
            } else if(keyMode == 'T') { // Translação
                posZ -= stepTranslate;
            }
            break;

        case Qt::Key_Left:
            if(keyMode == 'R') { // Rotação
                angleX -= stepRotate;
                angleX = angleX < 0 ? angleX + 360 : angleX;
            } else if(keyMode == 'T') { // Translação
                posX -= stepTranslate;
            }
            break;

        case Qt::Key_Right:
            if(keyMode == 'R') { // Rotação
                angleX += stepRotate;
                angleX = angleX > 360 ? angleX - 360 : angleX;
            } else if(keyMode == 'T') { // Translação
                posX += stepTranslate;
            }
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
