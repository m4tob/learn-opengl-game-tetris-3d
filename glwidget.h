/*
 * UNIVERSIDADE ESTADUAL DE FEIRA DE SANTANA - UEFS
 * Engenharia da Computação
 * TEC431 - Computação Gráfica
 *
 * Trabalho Final - 2019.2E
 *
 * Alunos:
 *      Matheus Oliveira Borges <matob@live.com>
 *      Luiz Ricardo Inês de Souza <lzricardo.ecomp@gmail.com>
 *
 */
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

private:
    GLuint _textureBorder;
    GLuint *_texturesBox; // Uma para cada um dos tipos de peça
    void setupTextures();
    GLuint loadTexture(QString image);
    void setupLighting();

    GLfloat cubeSize;
    GLuint cubeListIndex;
    GLfloat stepRotate, stepTranslate;
    GLfloat angleX, angleY, posX, posY, posZ;
    void drawCube();
    void paintBoardBorder();
    void paintBoardContent();
    void gameOver();

    QTimer *timer; // Timer para a taxa de atualização da tela (framerate)
    QTimer *gameCicleTimer; // Timer para a taxa de execução do jogo (ciclo de jogo)
    GLint cicleTime;
    Board board;

public Q_SLOTS:
    virtual void updateGameCicle();
};

#endif // GLWIDGET_H
