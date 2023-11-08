#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include <QColor>
#include <QObject>

class Board;
class Piece;

class Player : public QObject
{
    Q_OBJECT

public:
    Player(const int numPieces, const QColor color, Board* owner);

    bool isPlaying() const {return playing;}

public slots:
    void startTurn();
    void finishMoving();

signals:
    void endTurn();

private:
    Board* board;
    QList<Piece*> pieces;
    QColor pieceColor;
    int maxPieces;
    int placedPieces;
    bool playing;

};

#endif // PLAYER_H
