#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>

#include <QWidget>
#include <QImage>
#include <QList>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    QList<QList<float>> spaces;
    QList<QList<int>> edges;
    int num_black_pieces;
    int num_white_pieces;

    void drawBoard();
    void get_pieces_nodes_edges_from_file(const std::string file_dir);
    bool extract_player_pieces(const std::string& line, const std::string& key, int& num_pieces);
    QList<float> extract_node_coordinates(const std::string line);
    QList<int> extract_edge_coordinates(const std::string line);
};

#endif // BOARD_H
