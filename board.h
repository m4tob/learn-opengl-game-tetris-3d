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
#ifndef BOARD_H
#define BOARD_H

#include <QBasicTimer>
#include <QGLWidget>
#include <QTime>
#include <QHash>

// Enum com as peças possíveis para utilização no jogo
enum Piece {
        Point, ZRight, ZLeft, Line, T, Square, LRight, LLeft,
        PIECE_COUNT // utilizado para identificar a quantidade de peças
    };

// Matriz que identifica o desenho inicial de cada uma das peças
static constexpr int pieceTable[PIECE_COUNT][4][2] = {
        { {0, 0}, {0, 0}, {1, 0}, {0, 0} }, // Point
        { {0, 1}, {1, 1}, {1, 0}, {0, 0} }, // ZRight
        { {1, 0}, {1, 1}, {0, 1}, {0, 0} }, // ZLeft
        { {1, 0}, {1, 0}, {1, 0}, {1, 0} }, // Line
        { {0, 1}, {1, 1}, {0, 1}, {0, 0} }, // T
        { {0, 0}, {1, 1}, {1, 1}, {0, 0} }, // Square
        { {1, 1}, {0, 1}, {0, 1}, {0, 0} }, // LRight
        { {0, 1}, {0, 1}, {1, 1}, {0, 0} }  // LLeft
    };

class Board {

public:
    Board(int width, int height);
    ~Board();

    int width, height;
    int **currentBoard; // Matriz que armazena as peças que já foram posicionadas e a peça atual
    Piece nextPiece;
    int score;
    bool gameOver;

    void nextGameCicle();

    void rotate();
    void moveLeft();
    void moveRight();
    void moveDown();

private:
    int **droppedBoard; // Matriz que armazena as peças que já foram posicionadas

    Piece piece;
    int pieceStateSize = 4;
    int pieceState[4][4]; // Armazena o estado atual da peça
    int pieceX, pieceY, pieceRotation;

    bool checkingLines;

    void reset();
    void resetPiece();
    Piece randomPiece();
    void updatePieceState();
    void executeRotation();

    void refreshCurrentBoard();
    void dropPiece();
    bool checkLines();
    void removeLine(int y);
    void registerScore();
    void checkEndGame();
};

#endif // BOARD_H
