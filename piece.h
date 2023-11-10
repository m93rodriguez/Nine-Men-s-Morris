#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QList>
#include <QColor>

class Board;
class Player;

class Piece : public QWidget
{
    Q_OBJECT
public:
    explicit Piece(Player* owner, const QColor pieceColor, Board *parent = nullptr);

    void movePiece(QPoint newPosition);
    const QPoint centerPos() const;

    bool isFirstTurn() const {return firstTurn;}
    size_t getSpaceIndex() const {return spaceIndex;}
    Player* getPlayer() const {return player;}
    int numberTriplets() const {return numTriplets;}

    void addToTriplet() {numTriplets++;}
    void removeFromTriple() {if (numTriplets > 0) numTriplets--;}

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    QPixmap visual;
    QPoint oldPosition;
    QPointF relativeBoardPosition;
    Board* board;
    bool picked;
    size_t spaceIndex;
    Player* player;
    bool firstTurn;
    int numTriplets;

    bool isValidPosition(QPoint targetPosition);
    QPixmap createPixmap(int size, QColor color);

public slots:
    void resizePiecePosition();

signals:
    void getsPickedUp(Piece* piece);
    void placedInSpace(Piece* piece, size_t spaceInd);
    void returnedToSpace(Piece* piece, size_t spaceInd);
    void getsEliminated(Piece* piece, size_t spaceInd);

};

#endif // PIECE_H
