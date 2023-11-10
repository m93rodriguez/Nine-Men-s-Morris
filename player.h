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
    bool isAttacked() const {return underAttack;}
    bool moveIsAllowed(const Piece* piece, const size_t newSpaceInd, const size_t oldSpaceInd);
    bool hasAllTriplets() const {return allPiecesProtected;}

public slots:
    void startTurn();
    void finishMoving(Piece* movedPiece);
    void becomesAttacked();
    void eliminatePiece(Piece* eliminatedPiece, size_t spaceInd);
    void finishElimination();


signals:
    void endTurn();
    void eliminationStarts();
    void pieceRemoved(Piece* removedPiece, size_t spaceInd);

private:
    Board* board;
    QList<Piece*> pieces;
    QColor pieceColor;
    int maxPieces;
    int placedPieces;
    int currentPieces;
    bool playing;
    bool freeMoves;
    bool firstPhase;
    bool underAttack;
    bool allPiecesProtected;
    QList<QList<Piece*>> triplets;

    bool checkNewTriplet(Piece *movedPiece);
};

#endif // PLAYER_H
