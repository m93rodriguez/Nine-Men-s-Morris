#include "board.h"

#include <string>
#include <fstream>
#include <iostream>

#include <QMouseEvent>
#include <QPainter>

#include <QFileDialog>


#include <QDebug>
#include <locale>

Board::Board(QWidget *parent)
    : QWidget{parent}, image(500,500, QImage::Format_RGB32)
{
    image.fill(qRgb(200, 120, 200));
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    QString fileName = "/home/martin/Projects/Nine-Men-s-Morris/Board_Config/Classic_Nine_Men_Morris_corrected.txt";
    std::setlocale(LC_NUMERIC, "en_US.UTF-8");
    get_pieces_nodes_edges_from_file(fileName.toStdString());
    qDebug() << edges[10];
}

void Board::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
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
            edges.append(extract_edge_coordinates(line));
        }
        if (node_list){
            if (line.find("EDGES") != std::string::npos){
                node_list = false;
                edge_list = true;
                continue;
            }
            spaces.append(extract_node_coordinates(line));
        }

        if (extract_player_pieces(line, "BLACK_PIECES", num_black_pieces)) continue;
        if (extract_player_pieces(line, "WHITE_PIECES", num_black_pieces)) continue;
        if (line.find("SPACES") != std::string::npos){
            node_list = true;
            continue;
        }

    }
}

QList<float> Board::extract_node_coordinates(const std::string line)
{
    const std::size_t index1 = line.find(',');
    const std::size_t index2 = line.find(',', index1 + 1);
    QList<float> coordinates;
    coordinates.append(std::stof(line.substr(index1+1, index2-index1-1)));
    coordinates.append(std::stof(line.substr(index2+1)));

    return coordinates;
}

QList<int> Board::extract_edge_coordinates(const std::string line)
{
    const std::size_t index1 = line.find(',');
    const std::size_t index2 = line.find(',', index1 + 1);
    QList<int> coordinates;
    coordinates.append(std::stoi(line.substr(index1+1, index2-index1-1)));
    coordinates.append(std::stoi(line.substr(index2+1)));

    return coordinates;
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

