#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "visualwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pbPlaceBall_clicked();
    void on_pbPlaceCup_clicked();

    void on_pEndTurn_clicked();

    void on_pbSearch_clicked();

private:
    Ui::MainWindow *ui;
    VisualWidget *canvas;
};

#endif // MAINWINDOW_H
