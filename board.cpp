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
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            droppedBoard[x][y] = 0;
            currentBoard[x][y] = 0;
        }
    }

    score = 0;
    checkingLines = false;
    nextPiece = randomPiece();
    resetPiece();
}

void Board::resetPiece() {
    piece = nextPiece;
    pieceX = (width - pieceStateSize) / 2;
    pieceY = 0;
    pieceRotation = 0;
    nextPiece = randomPiece();
    updatePieceState();
    refreshCurrentBoard();
}

Piece Board::randomPiece() {
    return Piece(QRandomGenerator::global()->bounded(PIECE_COUNT) + 1);
}

void Board::updatePieceState() {
    for(int x = 0; x < pieceStateSize; x++) {
        for(int y = 0; y < pieceStateSize; y++) {
            pieceState[x][y] = 0;
        }
    }
    for(int x = 0; x < pieceStateSize; x++) {
        for(int y = 0; y < 2; y++) {
            pieceState[x][y] = pieceTable[piece][x][y] > 0 ? piece : 0;
        }
    }

    executeRotation();

    refreshCurrentBoard();
}

using namespace std;

void Board::executeRotation() {
    if(pieceRotation == 0)
        return;
    if(piece == Point || piece == Square)
        return;
    if(pieceRotation % 2 == 0 && (piece == Line || piece == ZLeft || piece == ZRight))
        return;

    if(piece == Line) {
        pieceState[0][0] = piece;
        pieceState[0][1] = piece;
        pieceState[0][2] = piece;
        pieceState[0][3] = piece;
        pieceState[1][0] = 0;
        pieceState[2][0] = 0;
        pieceState[3][0] = 0;
        return;
    }

    int aux;
    for(int r = 0; r < pieceRotation; r++) {
        aux = pieceState[2][0];
        pieceState[2][0] = pieceState[0][0];
        pieceState[0][0] = pieceState[0][2];
        pieceState[0][2] = pieceState[2][2];
        pieceState[2][2] = aux;

        aux = pieceState[2][1];
        pieceState[2][1] = pieceState[1][0];
        pieceState[1][0] = pieceState[0][1];
        pieceState[0][1] = pieceState[1][2];
        pieceState[1][2] = aux;
    }
}

void Board::refreshCurrentBoard() {
    // Copia a board para currentBoard
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            currentBoard[x][y] = droppedBoard[x][y];
        }
    }

    if(checkingLines)
        return;

    // Copia a peça para currentBoard
    for(int x = 0; x < pieceStateSize && (pieceX + x < width); x++) {
        for(int y = 0; y < pieceStateSize && (pieceY + y < height); y++) {
            if(pieceState[x][y] > 0 && pieceState[x][y] <= PIECE_COUNT)
                currentBoard[pieceX + x][pieceY + y] = pieceState[x][y];
        }
    }
}

bool Board::checkLines() {
    bool filledRow;
    for(int y = (height - 1); y >= 0; y--) {
        filledRow = true;
        for(int x = 0; x < width; x++)
            filledRow &= droppedBoard[x][y] > 0;

        if(filledRow) {
            removeLine(y);
            registerScore();
            return true;
        }
    }

    checkingLines = false;
    return false;
}

void Board::removeLine(int line) {
    for(int y = line; y > 0; y--)
        for(int x = 0; x < width; x++)
            droppedBoard[x][y] = droppedBoard[x][y - 1];

    for(int x = 0; x < width; x++)
        droppedBoard[x][0] = 0;

    refreshCurrentBoard();
}

void Board::registerScore() {
    score++;
}

void Board::updateGame() {
    if(!checkingLines || !checkLines())
        moveDown();
}

void Board::dropPiece() {
    for(int x = 0; x < pieceStateSize && (pieceX + x < width); x++) {
        for(int y = 0; y < pieceStateSize && (pieceY + y < height); y++) {
            if(pieceState[x][y] > 0)
                droppedBoard[pieceX + x][pieceY + y] = pieceState[x][y];
        }
    }

    checkingLines = true;

    resetPiece();
}

void Board::rotate() {
    if(checkingLines)
        return;

    pieceRotation++;
    if(pieceRotation == 4)
        pieceRotation = 0;
    updatePieceState();
}

void Board::moveLeft() {
    if(checkingLines)
        return;

    bool hasPieceInLeft = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height)
            break;
        for(int x = 0; x < pieceStateSize; x++) {
            if(pieceState[x][y] > 0) {
                if((pieceX + x) > 1 && droppedBoard[pieceX + x - 1][pieceY + y] > 0)
                    hasPieceInLeft = true;
                break;
            }
        }
    }

    // Verifica se há colunas da peça à esquerda vazias
    int offset = 0;
    if(pieceState[0][0] == 0 && pieceState[0][1] == 0)
        offset = 1;
    if(offset == 1 && pieceState[1][0] == 0 && pieceState[1][1] == 0)
        offset = 2;
    if(offset == 2 && pieceState[2][0] == 0 && pieceState[2][1] == 0)
        offset = 3;
    bool hasWallInLeft = (pieceX + offset) <= 0;

    if(!hasPieceInLeft && !hasWallInLeft)
        pieceX--;

    refreshCurrentBoard();
}

void Board::moveRight() {
    if(checkingLines)
        return;

    bool hasPieceInRight = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height)
            break;
        for(int x = pieceStateSize - 1; x >= 0; x--) {
            if(pieceState[x][y] > 0) {
                if((pieceX + x) < (width - 1) && droppedBoard[pieceX + x + 1][pieceY + y] > 0)
                    hasPieceInRight = true;
                break;
            }
        }
    }

    // Verifica se há colunas da peça à direita vazias
    int offset = 0;
    if(pieceState[3][0] == 0 && pieceState[3][1] == 0)
        offset = 1;
    if(offset == 1 && pieceState[2][0] == 0 && pieceState[2][1] == 0)
        offset = 2;
    if(offset == 2 && pieceState[1][0] == 0 && pieceState[1][1] == 0)
        offset = 3;
    bool hasWallInRight = ((pieceX + pieceStateSize) - offset) >= width;

    if(!hasPieceInRight && !hasWallInRight)
        pieceX++;

    refreshCurrentBoard();
}

void Board::moveDown() {
    if(checkingLines)
        return;

    bool nextRowIsFilled = false;
    for(int x = 0; x < pieceStateSize; x++) {
        if((pieceX + x) < 0)
            continue;
        if((pieceX + x) >= width)
            break;

        for(int y = pieceStateSize - 1; y >= 0; y--) {
            if(pieceState[x][y] > 0 && ((pieceY + y + 1) >= height || droppedBoard[pieceX + x][pieceY + y + 1] > 0))
                nextRowIsFilled = true;
        }
    }
    if(!nextRowIsFilled && pieceY < (height - 1))
        pieceY++;

    if(nextRowIsFilled)
        dropPiece();

    refreshCurrentBoard();
}
