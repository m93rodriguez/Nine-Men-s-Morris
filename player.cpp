#include "player.h"
#include "board.h"
#include "piece.h"

#include <QObject>

Player::Player(int numPieces, QColor color, Board* owner)
    : QObject(owner), pieceColor(color), maxPieces(numPieces), placedPieces(0), board(owner), playing(true)
{
}

void Player::startTurn()
{
    playing = true;
    qDebug() << "Turn started : " << this << " " << maxPieces;
    if (placedPieces < maxPieces){
        Piece* newPiece = new Piece(this, pieceColor, board);
        pieces.append(newPiece);
        connect(newPiece, &Piece::placedInSpace, this, &Player::finishMoving);
        connect(newPiece, &Piece::placedInSpace, board, &Board::releasePiece);
        placedPieces++;
        qDebug() << "Piece created. Visible: " << newPiece->isVisible();
    }
}

void Player::finishMoving()
{
    playing = false;
    qDebug() << "Turn ended: " << this;
    emit endTurn();
}
