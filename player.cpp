#include "player.h"
#include "board.h"
#include "piece.h"

#include <QObject>

Player::Player(int numPieces, QColor color, Board* owner)
    : QObject(owner), pieceColor(color), maxPieces(numPieces), placedPieces(0), board(owner),
    playing(true), firstPhase(true), currentPieces(0), underAttack(false),
    allPiecesProtected(false), attacking(false)
{
    connect(this, &Player::pieceRemoved, board, &Board::removePiece);
}

void Player::startTurn()
{
    playing = true;
    freeMoves = false;
    underAttack = false;
    allPiecesProtected = false;

    if (placedPieces == maxPieces)
        firstPhase = false;

    if (firstPhase)
    {
        freeMoves = true;
        Piece* newPiece = new Piece(this, pieceColor, board);
        pieces.append(newPiece);

        placedPieces++;
        currentPieces++;

        return;
    }

    qDebug() << currentPieces;

    if (currentPieces <= 3){
        qDebug() << "Less than 3 pieces";
        freeMoves = true;
    }

}

void Player::eliminatePiece(Piece* eliminatedPiece, size_t spaceInd)
{
    qDebug() << "Start Piece removal in Player";
    size_t pieceId = pieces.indexOf(eliminatedPiece);
    pieces.remove(pieceId);

    for (int ind = 0; ind < pieces.size(); ind++){
        pieces[ind]->removeAura();
    }

    if (triplets.size() > 0){
        qDebug() << "Num triplets: " << triplets.size();
        for (int ind = triplets.size() - 1; ind >= 0; ind--){
            qDebug() << ind;
            if (!triplets[ind].contains(eliminatedPiece)) continue;
            for (size_t piece = 0; piece < 3; piece++){
                if (triplets[ind][piece] == eliminatedPiece) continue;
                triplets[ind][piece]->removeFromTriple();
            }
            triplets.remove(ind);
        }
    }
    currentPieces--;
    qDebug() << "Ended Piece removal un Player";
    delete eliminatedPiece;
    emit pieceRemoved(eliminatedPiece, spaceInd);

}

bool Player::moveIsAllowed(const Piece* piece, const size_t newSpaceInd, const size_t oldSpaceInd)
{
    bool ret = false;
    if (firstPhase){
        ret = (piece->isFirstTurn()) ? true: false;
        return ret;
    }

    if (freeMoves) return true;

    QList<int> testEdge(2, oldSpaceInd);

    if (newSpaceInd < oldSpaceInd)
        testEdge[0] = newSpaceInd;
    else
        testEdge[1] = newSpaceInd;

    return board->getEdges().contains(testEdge);
}

void Player::finishMoving(Piece* movedPiece)
{
    if (triplets.size() > 0) {
        int trip = triplets.size();
        while (movedPiece->numberTriplets() > 0){
            trip--;
            if (!triplets[trip].contains(movedPiece)) continue;
            for (int ind = 0; ind < triplets[trip].size(); ind++){
                triplets[trip][ind]->removeFromTriple();
            }
        }
    }

    if (checkNewTriplet(movedPiece)){
        attacking = true;
        emit eliminationStarts();
        return;
    }

    playing = false;
    emit endTurn();
}

void Player::finishElimination()
{
    playing = false;
    attacking = false;
    allPiecesProtected = true;
    for (int ind = 0; ind < pieces.size(); ind++){
        if (pieces[ind]->numberTriplets() == 0){
            allPiecesProtected = false;
            break;
        }
    }

    emit endTurn();
}

void Player::becomesAttacked()
{
    underAttack = true;
    for (int ind = 0; ind < pieces.size(); ind++){
        if ((pieces[ind]->numberTriplets() > 0) && (!allPiecesProtected)){
            continue;
        }
        pieces[ind]->setUnderAttack(true);
        pieces[ind]->addAura(Qt::red);
    }
}

bool Player::checkNewTriplet(Piece *movedPiece)
{
    qDebug() << "Check for Tiplet";
    bool ret = false;
    const QList<QList<int>> edges = board->getEdges();

    for (size_t edge1 = 0; edge1 < edges.size(); edge1++){
        size_t i = edges[edge1].indexOf(movedPiece->getSpaceIndex());

        if (i == -1) continue;

        size_t j = (i + 1) % 2;
        Piece* secondPiece = board->pieceInSpace(edges[edge1][j]);

        if (!secondPiece) continue;
        if (secondPiece->getPlayer() != movedPiece->getPlayer()) continue;

        for (size_t edge2 = 0; edge2 < edges.size(); edge2++){
            if (edge2 == edge1) continue;
            size_t k = edges[edge2].indexOf(secondPiece->getSpaceIndex());
            if (k == -1) {
                k = edges[edge2].indexOf(movedPiece->getSpaceIndex());
            }
            if (k==-1) continue;

            Piece* thirdPiece = board->pieceInSpace(edges[edge2][(k+1)%2]);

            if (!thirdPiece) continue;
            if (thirdPiece->getPlayer() != movedPiece->getPlayer()) continue;

            QPoint P1 = board->getSpacePoint(movedPiece->getSpaceIndex());
            QPoint P2 = board->getSpacePoint(secondPiece->getSpaceIndex());
            QPoint P3 = board->getSpacePoint(thirdPiece->getSpaceIndex());

            int collinearity = P1.x()*(P2.y() - P3.y());
            collinearity += P2.x()*(P3.y() - P1.y());
            collinearity += P3.x()*(P1.y() - P2.y());

            if (!collinearity){
                QList<Piece*> newTriplet = {movedPiece, secondPiece, thirdPiece};
                movedPiece->addToTriplet();
                secondPiece->addToTriplet();
                thirdPiece->addToTriplet();
                triplets.append(newTriplet);

                ret = true;
                qDebug() << "Tiplet found";
            }

        }



    }

    return ret;
}
