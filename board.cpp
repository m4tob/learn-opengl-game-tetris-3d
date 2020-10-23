#include "board.h"

#include <QtCore>

Board::Board(int width, int height) {
    this->width = width;
    this->height = height;

    droppedBoard = (int **) calloc(width, sizeof (int));
    currentBoard = (int **) calloc(width, sizeof (int));
    for(int x = 0; x < width; x++) {
        droppedBoard[x] = (int *) calloc(height, sizeof (int));
        currentBoard[x] = (int *) calloc(height, sizeof (int));
    }

    reset();
}

void Board::reset() {
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++) {
            droppedBoard[x][y] = 0;
            currentBoard[x][y] = 0;
        }

    score = 0;
    nextPiece = randomPiece();
    resetPiece();
}

void Board::resetPiece() {
    piece = nextPiece;
    pieceX = (width - 4) / 2;
    pieceY = 0;
    nextPiece = randomPiece();
    updatePieceState();
}

Piece Board::randomPiece() {
    return Piece(QRandomGenerator::global()->bounded(PIECE_COUNT) + 1);
}

void Board::updatePieceState() {
}

int countY = 0, countX = 0;
void Board::updateGame() {
    droppedBoard[countX][countY] = QRandomGenerator::global()->bounded(PIECE_COUNT) + 1;
    countX++;
    if(countX == width) {
        countX = 0;
        countY++;
        if(countY == height)
            countY = 0;
    }

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            currentBoard[x][y] = droppedBoard[x][y];
        }
    }
}

void Board::dropPiece() {
    // TODO - Adiciona o piece no board
    resetPiece();
}

void Board::rotate() {
    pieceRotation = (pieceRotation < 3) ? (pieceRotation + 1) : 0;
    updatePieceState();
}

void Board::moveLeft() {
    if(pieceX > 0)
        pieceX--;
}

void Board::moveRight() {
    if(pieceX < (width - 1))
        pieceX++;
}

void Board::moveDown() {
    bool rowIsEmpty = false;
    if(rowIsEmpty && pieceY < (height - 1))
        pieceY++;

    if(!rowIsEmpty)
        dropPiece();
}
