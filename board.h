#ifndef BOARD_H
#define BOARD_H

enum Piece { Point, ZRight, ZLeft, Line, T, Square, LRight, LLeft, PIECE_COUNT };

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

#include <QBasicTimer>
#include <QGLWidget>
#include <QTime>
#include <QHash>

class Board
{

public:
    Board(int width, int height);

    int width, height;
    int **currentBoard;

    int **getCurrentBoard();

    void rotate();
    void moveLeft();
    void moveRight();
    void moveDown();

    Piece nextPiece;
    int score;

public Q_SLOTS:
    virtual void updateGame();

private:
    int **droppedBoard;

    Piece piece;
    int pieceStateSize = 4;
    int pieceState[4][4];
    int pieceRotation; // 0 - 3
    int pieceX;
    int pieceY;

    void reset();
    void resetPiece();
    Piece randomPiece();

    void updatePieceState();
    void executeRotation();
    void refreshCurrentBoard();

    void dropPiece();
};

#endif // BOARD_H
