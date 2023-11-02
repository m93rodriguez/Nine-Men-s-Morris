#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPoint>
#include <QList>
#include <QColor>

class Board;

class Piece : public QWidget
{
    Q_OBJECT
public:
    explicit Piece(Board *parent = nullptr);

    void movePiece(QPoint newPosition);

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

    bool isValidPosition(QPoint targetPosition);
    QPixmap createPixmap(int size, QColor color);

public slots:
    void resizePiecePosition(int factor_x, int factor_y);

signals:
    void getsPickedUp(Piece* piece);

};

#endif // PIECE_H
