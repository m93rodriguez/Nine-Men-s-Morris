#include "board.h"
#include "piece.h"
#include "player.h"

#include <string>
#include <fstream>
#include <iostream>

#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QImage>
#include <QPainterPath>
#include <QColor>
#include <QList>

#include <QFileDialog>

#include <QDebug>
#include <locale>


Board::Board(QWidget *parent)
    : QWidget{parent}, boardSize(QSize(500,500)), imageOffset(QPoint(0,0)), movingPiece(nullptr)
{
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");

    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    QString fileName = "/home/martin/Projects/Nine-Men-s-Morris/Board_Config/Classic_Nine_Men_Morris_corrected.txt";

    get_pieces_nodes_edges_from_file(fileName.toStdString());
    spacePieces = QList<Piece*>(relativeSpaces.size(), nullptr);
    setMinimumSize(boardSize);

    player[0] = new Player(num_black_pieces, QColor(0, 0, 0), this);
    player[1] = new Player(num_white_pieces, QColor(255, 255, 255), this);

    connect(player[0], &Player::endTurn, player[1], &Player::startTurn);
    connect(player[1], &Player::endTurn, player[0], &Player::startTurn);

    player[1]->startTurn();
}

void Board::mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Board";
}

void Board::drawBoard()
{
    image.fill(qRgb(200, 200, 120));
    QPainter painter(&image);
    QList<QPoint>::iterator space;
    QPainterPath circles;
    QColor myPenColor = Qt::blue;
    int myPenWidth = 4;
    int spaceRadius = 10;

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    for (space = pixelSpaces.begin(); space != pixelSpaces.end(); space++){
        circles.addEllipse(*space, spaceRadius, spaceRadius);
    }
    painter.fillPath(circles, Qt::blue);

    for (int edge = 0; edge < edges.size(); edge++){
        painter.drawLine(pixelSpaces[edges[edge][0]], pixelSpaces[edges[edge][1]]);
    }
}

void Board::resizeNodes(int factor_x, int factor_y)
{
    if (pixelSpaces.isEmpty()){
        for (size_t ind = 0; ind < relativeSpaces.size(); ind++){
            pixelSpaces.append(QPoint(0,0));
        }
    }

    for (size_t ind = 0 ; ind < relativeSpaces.size(); ind++){
        pixelSpaces[ind].setX( static_cast<int>(relativeSpaces[ind].x()*factor_x + imageOffset.x()));
        pixelSpaces[ind].setY( static_cast<int>(relativeSpaces[ind].y()*factor_y + imageOffset.y()));
    }

}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void Board::resizeEvent(QResizeEvent *event)
{
    const QSize newSize = event->size();
    image = QImage(newSize, QImage::Format_RGB32);
    const int scaleFactor = qMin(newSize.width(), newSize.height());

    boardSize.setWidth(scaleFactor);
    boardSize.setHeight(scaleFactor);

    imageOffset = QPoint((newSize.width() - boardSize.width())/2,
                          (newSize.height() - boardSize.height())/2);

    resizeNodes(scaleFactor, scaleFactor);
    drawBoard();

    QWidget::resizeEvent(event);
    emit isResized(scaleFactor, scaleFactor);
}

void Board::get_pieces_nodes_edges_from_file(const std::string file_dir)
{
    std::ifstream stream(file_dir);
    if (!stream) {return; } // To complete in case of file error

    std::string line;
    std::string key;
    std::size_t index;
    bool node_list = false;
    bool edge_list = false;

    while (std::getline(stream, line))
    {
        if (edge_list){
            edges.append(extract_edges(line));
        }
        if (node_list){
            if (line.find("EDGES") != std::string::npos){
                node_list = false;
                edge_list = true;
                continue;
            }
            relativeSpaces.append(extract_coordinates(line));
        }

        if (extract_player_pieces(line, "BLACK_PIECES", num_black_pieces)) continue;
        if (extract_player_pieces(line, "WHITE_PIECES", num_white_pieces)) continue;
        if (line.find("SPACES") != std::string::npos){
            node_list = true;
            continue;
        }

    }
}

QPointF Board::extract_coordinates(const std::string line)
{
    const std::size_t index1 = line.find(',');
    const std::size_t index2 = line.find(',', index1 + 1);

    const float x = std::stof(line.substr(index1+1, index2-index1-1));
    const float y = std::stof(line.substr(index2+1));

    return QPointF(x, y);
}

QList<int> Board::extract_edges(const std::string line)
{
    const std::size_t index1 = line.find(',');
    const std::size_t index2 = line.find(',', index1 + 1);
    QList<int> node_list;
    node_list.append(std::stoi(line.substr(index1+1, index2-index1-1)));
    node_list.append(std::stoi(line.substr(index2+1)));

    return node_list;
}

bool Board::extract_player_pieces(const std::string& line, const std::string& key, int& num_pieces)
{
    std::size_t index;
    char const* digits = "0123456789";
    index = line.find(key);
    if (index != std::string::npos){
        index = line.find_first_of(digits);
        num_pieces = std::stoi(line.substr(index));
        return true;
    }
    return false;
}

void Board::pickUpPiece(Piece* pickedPiece)
{
    movingPiece = pickedPiece;

}

void Board::fillSpace(Piece* piece, const size_t spaceInd)
{
    spacePieces[spaceInd] = piece;
}

Piece* Board::pieceInSpace(size_t ind) const
{
    if ((ind < 0) || (ind >= spacePieces.size())) {return nullptr;}

    return spacePieces.at(ind);
}

void Board::mouseMoveEvent(QMouseEvent *event)
{
    if (movingPiece)
        movingPiece->movePiece(event->pos());
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{

}

void Board::releasePiece()
{
    movingPiece = nullptr;
}
