#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMouseEvent>
#include <vector>
#include "visualwidget.hpp"
#include <rcbigcar/board.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void update();

    void on_pWhoAmI_clicked();

    void on_pTeamOk_clicked();

    //void on_pTestSeq_clicked();

    void on_pbTestSeq_clicked();

private:
    Ui::MainWindow *ui;
    VisualWidget *canvas;
    QTimer *timer;
    QTimer *ui_update_timer;

    void BoardCallback(const rcbigcar::board::ConstPtr &board);

    void UpdateStrategy();
};

#endif // MAINWINDOW_H
