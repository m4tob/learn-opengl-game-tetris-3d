#ifndef BOARD_H
#define BOARD_H

enum Piece { ZRight, ZLeft, Line, T, Square, LRight, LLeft, PIECE_COUNT };

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
    int pieceState[4][4];
    int pieceRotation; // 0 - 3
    int pieceX;
    int pieceY;

    void reset();
    void resetPiece();
    Piece randomPiece();

    void updatePieceState();

    void dropPiece();
};

#endif // BOARD_H
