#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>

#include <QWidget>
#include <QImage>
#include <QList>
#include <QPointF>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QSize boardSize;
    QPoint imageOffset;
    QImage image;
    QList<QPointF> relativeSpaces;
    QList<QPoint> pixelSpaces;
    QList<QList<int>> edges;
    int num_black_pieces;
    int num_white_pieces;

    void drawBoard();
    void get_pieces_nodes_edges_from_file(const std::string file_dir);
    bool extract_player_pieces(const std::string& line, const std::string& key, int& num_pieces);
    QPointF extract_coordinates(const std::string line);
    QList<int> extract_edges(const std::string line);
    void resizeNodes(int factor_x, int factor_y);
};


#endif // BOARD_H
