#include "mainwindow.h"
#include "board.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    board = new Board(this);
    setCentralWidget(board);
}

MainWindow::~MainWindow()
{
}

