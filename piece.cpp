#include "piece.h"
#include "board.h"

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QList>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QMouseEvent>

QPointF operator/(const QPoint absPoint, const QSize box)
{
    QPointF relPoint(absPoint);
    relPoint.rx() /= box.width();
    relPoint.ry() /= box.height();
    return relPoint;

}


Piece::Piece(Board *parent)
    : QWidget{parent}, relativeBoardPosition(QPointF(0.5, 0.5)), board(parent)
{
    visual = createPixmap(50, QColor(0, 0, 0, 255));
    setFixedSize(50, 50);

    connect(this, &Piece::getsPickedUp, parent, &Board::pickUpPiece);
    connect(parent, &Board::isResized, this, &Piece::resizePiecePosition);
}

QPixmap Piece::createPixmap(int size, QColor color)
{
    QImage image(size, size, QImage::Format_ARGB32);
    QPainter painter(&image);
    QPainterPath circle;

    image.fill(QColor(0, 0, 0, 0));
    circle.addEllipse(image.rect());
    painter.fillPath(circle, color);
    return QPixmap::fromImage(image);

}

void Piece::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, visual, dirtyRect);
}

void Piece::movePiece(QPoint newPosition)
{
    QPoint centerPoint = QPoint(size().width()/2, size().height()/2);
    move(newPosition - centerPoint);
}

void Piece::resizePiecePosition(const int factor_x, const int factor_y)
{
    QPointF newPosition(relativeBoardPosition);
    newPosition.rx() *= factor_x;
    newPosition.ry() *= factor_y;
    movePiece(newPosition.toPoint() + board->get_boardOffset());
}

void Piece::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Piece";
    emit getsPickedUp(this);
}

void Piece::mouseMoveEvent(QMouseEvent* event)
{
    event->ignore();
}

void Piece::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint centerPoint = QPoint(size().width()/2, size().height()/2);
    relativeBoardPosition = (pos() + centerPoint - board->get_boardOffset())/board->get_boardSize();
    qDebug() << "PieceReleases";
}
