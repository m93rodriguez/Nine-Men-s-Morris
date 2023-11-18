#include "mainwindow.h"
#include "board.h"
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    board = new Board(this);
    setCentralWidget(board);
    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
}

void MainWindow::startNewGame()
{
    delete board;
    board = new Board(this);
    setCentralWidget(board);
}

void MainWindow::createActions()
{
    restartAct = new QAction("&New Game...", this);
    connect(restartAct, &QAction::triggered, this, &MainWindow::startNewGame);
}

void MainWindow::createMenus()
{
    options = this->addToolBar("Options");
    options->addAction(restartAct);
}

