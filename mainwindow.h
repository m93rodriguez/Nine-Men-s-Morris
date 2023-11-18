#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>

class Board;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

public slots:
    void startNewGame();

private:
    void createActions();
    void createMenus();

    QAction *restartAct;
    QToolBar *options;

    Board *board;


};
#endif // MAINWINDOW_H
