#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>

#include <QWidget>
#include <QImage>
#include <QList>
#include <QPointF>
#include <QSize>

class Piece;
class Player;

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    QPoint get_boardOffset() const {return imageOffset;}
    QSize get_boardSize() const {return boardSize;}
    QList<QPoint> get_spacePositions() const {return pixelSpaces;}
    QPoint getSpacePoint(size_t spaceInd) const;
    Piece* pieceInSpace(size_t ind) const;
    const QList<QList<int>>& getEdges() const {return edges;}

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


public slots:
    void pickUpPiece(Piece* pickedPiece);
    void fillSpace(Piece* piece, const size_t spaceInd);
    void releasePiece();
    void removePiece(Piece* removedPiece, size_t spaceInd);

signals:
    void isResized(const int factor_x, const int factor_y);

private:
    QSize boardSize;
    QPoint imageOffset;
    QImage image;
    QList<QPointF> relativeSpaces;
    QList<QPoint> pixelSpaces;
    QList<QList<int>> edges;
    QList<Piece*> spacePieces;
    int num_black_pieces;
    int num_white_pieces;
    Player* player[2];
    Piece* movingPiece;

    void drawBoard();
    void get_pieces_nodes_edges_from_file(const std::string file_dir);
    bool extract_player_pieces(const std::string& line, const std::string& key, int& num_pieces);
    QPointF extract_coordinates(const std::string line);
    QList<int> extract_edges(const std::string line);
    void resizeNodes(int factor_x, int factor_y);
};


#endif // BOARD_H
