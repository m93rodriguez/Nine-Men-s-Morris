#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include <QColor>

class Boad;
class Piece;

class Player
{
public:
    Player();

private:
    QList<Piece*> pieces;
    QColor pieceColor;
    int numPieces;

};

#endif // PLAYER_H
