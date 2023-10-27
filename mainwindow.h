#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Board;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

private slots:

private:

    Board *board;


};
#endif // MAINWINDOW_H
