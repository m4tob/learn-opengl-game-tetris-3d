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
#include <QtCore>
#include "board.h"
using namespace std;

// Constructor
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

// Destructor
Board::~Board() {
}

// Reseta o jogo limpando os tabuleiros e parâmetros
void Board::reset() {
    // Limpa os boards removendo todo o conteúdo
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            droppedBoard[x][y] = 0;
            currentBoard[x][y] = 0;
        }
    }

    score = 0;
    checkingLines = false;
    gameOver = false;
    nextPiece = randomPiece();
    resetPiece();
}

// Reseta as configurações da peça
void Board::resetPiece() {
    piece = nextPiece;
    pieceX = (width - pieceStateSize) / 2; // posiciona a peça próximo ao centro do eixo X
    pieceY = 0;
    pieceRotation = 0;
    nextPiece = randomPiece();
    updatePieceState();
    refreshCurrentBoard();
}

// Seleciona uma peça de forma "aleatória".
Piece Board::randomPiece() {
    return Piece(QRandomGenerator::global()->bounded(PIECE_COUNT));
}

/* Atualiza a matriz responsável por armazenar a
 * situação atual da peça
 */
void Board::updatePieceState() {
    // Limpa a matriz
    for(int x = 0; x < pieceStateSize; x++) {
        for(int y = 0; y < pieceStateSize; y++) {
            pieceState[x][y] = 0;
        }
    }

    // Carrega as configurações da peça a partir da matriz base (pieceTable).
    for(int x = 0; x < pieceStateSize; x++) {
        for(int y = 0; y < 2; y++) {
            pieceState[x][y] = pieceTable[piece][x][y] > 0 ? (piece + 1) : 0;
        }
    }

    executeRotation();

    refreshCurrentBoard();
}

/* Executa a rotação da matriz representando a peça de
 * acordo com o parâmetro pieceRotation.
 */
void Board::executeRotation() {
    // Se não há rotação na peça
    if(pieceRotation == 0)
        return;

    // Se a peça é um ponto ou um quadrado, não tem porque rotacionar
    if(piece == Point || piece == Square)
        return;

    // Se a peça é Line, ZLeft, ZRight e a rotação é par, ela volta para a situação inicial
    if(pieceRotation % 2 == 0 && (piece == Line || piece == ZLeft || piece == ZRight))
        return;

    // A rotação da peça Line é basicamente colocá-la em vertical
    if(piece == Line) {
        pieceState[0][0] = piece+1;
        pieceState[0][1] = piece+1;
        pieceState[0][2] = piece+1;
        pieceState[0][3] = piece+1;
        pieceState[1][0] = 0;
        pieceState[2][0] = 0;
        pieceState[3][0] = 0;
        return;
    }

    /* As demais rotações são executadas girando no sentido
       horário, trocando a posição dos elementos da matriz.
       Como a Line é a única peça de tamanho 4, a rotação é
       feita na sub-matriz 3x3 */
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

/* Atualiza a board atual com base em droppedBoard e na
 * configuração da peça atual (pieceState).
 */
void Board::refreshCurrentBoard() {
    // Copia droppedBoard para currentBoard
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            currentBoard[x][y] = droppedBoard[x][y];
        }
    }

    // Copia pieceState para currentBoard
    for(int x = 0; x < pieceStateSize && (pieceX + x < width); x++) {
        for(int y = 0; y < pieceStateSize && (pieceY + y < height); y++) {
            // Verificação para descartar possibilidade de lixo em memória
            if(pieceState[x][y] > 0 && pieceState[x][y] <= PIECE_COUNT)
                currentBoard[pieceX + x][pieceY + y] = pieceState[x][y];
        }
    }
}

// Posiciona a peça atual na board
void Board::dropPiece() {
    for(int x = 0; x < pieceStateSize && (pieceX + x < width); x++) {
        for(int y = 0; y < pieceStateSize && (pieceY + y < height); y++) {
            /* Verifica se a peça possui conteúdo no ponto x y para evitar
               remover uma peça já posicionada no tabuleiro.*/
            if(pieceState[x][y] > 0)
                droppedBoard[pieceX + x][pieceY + y] = pieceState[x][y];
        }
    }

    /* Caso uma linha seja removida durante a verificação, o jogo entra
       no modo de vefificação*/
    checkingLines = checkLines();

    // Cria uma nova peça
    resetPiece();
}

/* Verifica se o jogador completou alguma linha da board.
 * A remoção é executada uma vez por ciclo de jogo para
 * dar o efeito de animação.
 */
bool Board::checkLines() {
    bool filledRow;
    // Executa de baixo para cima da board
    for(int y = (height - 1); y >= 0; y--) {
        filledRow = true;
        // Verifica se toda a linha está completa
        for(int x = 0; x < width; x++)
            filledRow &= droppedBoard[x][y] > 0;

        if(filledRow) {
            removeLine(y);
            registerScore();
            /* retorna true para que no próximo ciclo esta
               função seja executada novamente */
            return true;
        }
    }

    /* Como não executou nenhuma remoção, não há porque
       continuar verificando*/
    checkingLines = false;
    return false;
}

/* Remove a linha passada como parâmetro e desloca todas
 * as outras anteriores a ela.
 */
void Board::removeLine(int line) {
    for(int y = line; y > 0; y--)
        for(int x = 0; x < width; x++)
            droppedBoard[x][y] = droppedBoard[x][y - 1];

    for(int x = 0; x < width; x++)
        droppedBoard[x][0] = 0;

    refreshCurrentBoard();
}

// Registra a pontuação do jogador
void Board::registerScore() {
    score++;
}

// Atualiza o estado atual do jogo para o próximo ciclo
void Board::nextGameCicle() {
    if(!checkingLines || !checkLines())
        moveDown();

    checkEndGame();
}

/* Verifica se o jogo chegou ao fim (quando a nova peça
 * não tem espaço para iniciar)
 */
void Board::checkEndGame() {
    for(int x = 0; x < pieceStateSize; x++) {
        for(int y = 0; y < pieceStateSize; y++) {
            if(pieceState[x][y] > 0 && droppedBoard[pieceX + x][pieceY + y] > 0) {// Game Over
                gameOver = true;
                return;
            }
        }
    }
}

// Rotaciona a peça uma vez (90º) no sentido horário
void Board::rotate() {
    if(checkingLines) // Caso esteja em fase de verificação, não é permitido movimenta a peça
        return;

    pieceRotation++;
    if(pieceRotation == 4) // Reseta a rotação, após 4 rotações
        pieceRotation = 0;

    updatePieceState();
}

// Move a peça atual um bloco para a esquerda
void Board::moveLeft() {
    if(checkingLines) // Caso esteja em fase de verificação, não é permitido movimenta a peça
        return;

    // Verifica se há algum bloco à esquerda da peça que impeça seu movimento
    bool hasPieceInLeft = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height) // Ignora verificações após o fim do tabuleiro
            break;
        for(int x = 0; x < pieceStateSize; x++) { // Da esquerda para a direita
            if(pieceState[x][y] > 0) { // Caso a peça possua um bloco em x y
                // Verifica se há um bloco já posicionado em X Y
                if((pieceX + x) > 1 && droppedBoard[pieceX + x - 1][pieceY + y] > 0)
                    hasPieceInLeft = true;
                break;
            }
        }
    }

    // Verifica se há parede esquerda de algum dos blocos da peça
    bool hasWallInLeft = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height)
            break;
        for(int x = 0; x < pieceStateSize; x++) {
            if(pieceState[x][y] > 0) {
                if((pieceX + x - 1) < 0)
                    hasWallInLeft = true;
                break;
            }
        }
    }

    // Se não há blocos ou parede à esquerda, move o bloco em uma unidade
    if(!hasPieceInLeft && !hasWallInLeft)
        pieceX--;

    refreshCurrentBoard();
}

// Move a peça atual um bloco para a direita
void Board::moveRight() {
    if(checkingLines) // Caso esteja em fase de verificação, não é permitido movimenta a peça
        return;

    // Verifica se há algum bloco à direita da peça que impeça seu movimento
    bool hasPieceInRight = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height) // Ignora verificações após o fim do tabuleiro
            break;
        for(int x = pieceStateSize - 1; x >= 0; x--) { // Da direita para a esquerda
            if(pieceState[x][y] > 0) { // Caso a peça possua um bloco em x y
                // Verifica se há um bloco já posicionado em X Y
                if((pieceX + x) < (width - 1) && droppedBoard[pieceX + x + 1][pieceY + y] > 0)
                    hasPieceInRight = true;
                break;
            }
        }
    }

    // Verifica se há parede esquerda de algum dos blocos da peça
    bool hasWallInRight = false;
    for(int y = 0; y < pieceStateSize; y++) {
        if((pieceY + y) >= height) // Ignora verificações após o fim do tabuleiro
            break;
        for(int x = pieceStateSize - 1; x >= 0; x--) {
            if(pieceState[x][y] > 0) {
                if((pieceX + x + 1) >= width)
                    hasWallInRight = true;
                break;
            }
        }
    }

    // Se não há blocos ou parede à direita, move o bloco em uma unidade
    if(!hasPieceInRight && !hasWallInRight)
        pieceX++;

    refreshCurrentBoard();
}

// Move a peça atual um bloco para baixo (independente do ciclo de jogo)
void Board::moveDown() {
    if(checkingLines) // Caso esteja em fase de verificação, não é permitido movimenta a peça
        return;

    // Verifica se há algum bloco ou parede abaixo da peça que impeça seu movimento
    bool nextRowIsFilled = false;
    for(int x = 0; x < pieceStateSize; x++) {
        if((pieceX + x) < 0) // Ignora a parte da peça que não está na board (colunas vazias)
            continue;
        if((pieceX + x) >= width) // Ignora verificações após o fim do tabuleiro
            break;

        for(int y = pieceStateSize - 1; y >= 0; y--) { // Verifica de baixo para cima
            bool existeBlocoNaPeca = pieceState[x][y] > 0;
            bool existeBloco = droppedBoard[pieceX + x][pieceY + y + 1] > 0;
            bool existeParede = (pieceY + y + 1) >= height;
            if(existeBlocoNaPeca && (existeBloco || existeParede))
                nextRowIsFilled = true;
        }
    }

    // Se não há blocos ou parede abaixo, move o bloco em uma unidade
    if(!nextRowIsFilled && pieceY < (height - 1))
        pieceY++;

    // Se há blocos ou parede abaixo, posiciona a peça na board.
    if(nextRowIsFilled)
        dropPiece();

    refreshCurrentBoard();
}
