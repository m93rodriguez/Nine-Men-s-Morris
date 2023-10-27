#include "mainwindow.h"
#include "board.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //resize(1500, 900);
    board = new Board;
    setCentralWidget(board);
}

MainWindow::~MainWindow()
{
}

