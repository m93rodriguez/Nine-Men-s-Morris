#include "piece.h"
#include "board.h"
#include "player.h"

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
#include <QBrush>
#include <QRadialGradient>

QPointF operator/(const QPoint absPoint, const QSize box)
{
    QPointF relPoint(absPoint);
    relPoint.rx() /= box.width();
    relPoint.ry() /= box.height();
    return relPoint;

}


Piece::Piece(Player* owner, const QColor pieceColor, Board *parent)
    : QWidget{parent}, relativeBoardPosition(QPointF(0.5, 0.5)), board(parent), player(owner),
    spaceIndex(-1), firstTurn(true), numTriplets(0), pieceSize(50), imageSize(70)
{
    pieceVisual = createPixmap(pieceColor);
    displayVisual = pieceVisual;

    addAura(Qt::blue);

    setFixedSize(imageSize, imageSize);
    resizePiecePosition();

    connect(this, &Piece::getsPickedUp, parent, &Board::pickUpPiece);

    connect(parent, &Board::isResized, this, &Piece::resizePiecePosition);

    connect(this, &Piece::placedInSpace, board, &Board::fillSpace);
    connect(this, &Piece::placedInSpace, player, &Player::finishMoving);
    connect(this, &Piece::returnedToSpace, board, &Board::releasePiece);

    connect(this, &Piece::getsEliminated, player, &Player::eliminatePiece);

    show();
}

QPixmap Piece::createPixmap(QColor color)
{
    int offset = (imageSize - pieceSize) / 2;

    QImage image(imageSize, imageSize, QImage::Format_ARGB32);
    QPainter painter(&image);
    QPainterPath circle;

    image.fill(QColor(0, 0, 0, 0));

    circle.addEllipse(offset, offset, pieceSize, pieceSize);
    painter.fillPath(circle, color);
    return QPixmap::fromImage(image);

}

void Piece::addAura(QColor color)
{
    QPainter painter(&displayVisual);

    QColor edgeColor = color;
    edgeColor.setAlpha(0);

    QRadialGradient gradient(imageSize/2, imageSize/2, imageSize/2);
    gradient.setColorAt(0, color);
    gradient.setColorAt(0.5, color);
    gradient.setColorAt(1, edgeColor);

    displayVisual.fill(QColor(0, 0, 0, 0));
    painter.fillRect(displayVisual.rect(), gradient);
    painter.drawPixmap(displayVisual.rect(), pieceVisual, pieceVisual.rect());
    update();
}

void Piece::removeAura()
{
    displayVisual.fill(QColor(0, 0, 0, 0));
    QPainter painter(&displayVisual);
    painter.drawPixmap(displayVisual.rect(), pieceVisual, pieceVisual.rect());
    update();
}

void Piece::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, displayVisual, dirtyRect);
}

void Piece::movePiece(QPoint newPosition)
{
    QPoint centerPoint = QPoint(size().width()/2, size().height()/2);
    move(newPosition - centerPoint);
}

void Piece::resizePiecePosition()
{
    const int factor = qMin(board->width(), board->height());
    QPointF newPosition(relativeBoardPosition);
    newPosition.rx() *= factor;
    newPosition.ry() *= factor;
    movePiece(newPosition.toPoint() + board->get_boardOffset());
}

void Piece::mousePressEvent(QMouseEvent* event)
{
    if (player->isAttacking()) return;
    if (player->isPlaying()){
        oldPosition = centerPos();
        picked = true;
        emit getsPickedUp(this);
        return;
    }

    if (player->isAttacked()){
        if (!underAttack) return;
        emit getsEliminated(this, spaceIndex);
    }
}

void Piece::mouseMoveEvent(QMouseEvent* event)
{
    event->ignore();
}

void Piece::mouseReleaseEvent(QMouseEvent* event)
{
    if (!picked) return;
    picked = false;

    QPoint targetPosition(QPoint(0,0));

    const QList<QPoint> &spaces = board->get_spacePositions();

    for (size_t spaceInd = 0; spaceInd < spaces.size(); spaceInd++){
        if ((spaces.at(spaceInd) - centerPos()).manhattanLength() < width()){

            if (board->pieceInSpace(spaceInd))
                break;
            if (!player->moveIsAllowed(this, spaceInd, spaceIndex))
                break;

            movePiece(spaces.at(spaceInd));
            oldPosition = spaces.at(spaceInd);
            spaceIndex = spaceInd;

            relativeBoardPosition = (spaces[spaceInd] - board->get_boardOffset())/board->get_boardSize();

            emit placedInSpace(this, spaceIndex);
            if (firstTurn){
                firstTurn = false;
                removeAura();
            }
            return;

        }
    }

    movePiece(oldPosition);
    emit returnedToSpace(this, spaceIndex);

    qDebug() << "Illegal move, Piece returned";
}

const QPoint Piece::centerPos() const
{
    return pos() + QPoint(size().width()/2, size().height()/2);
}
