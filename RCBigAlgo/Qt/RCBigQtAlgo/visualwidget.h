#ifndef VISUALWIDGET_H
#define VISUALWIDGET_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPlainTextEdit>

//! [0]

class VisualWidget : public QWidget
{
    Q_OBJECT

public:
    VisualWidget(QWidget *parent);

    void GameUpdate();
    void GamePlaceObject(int point);

    QPlainTextEdit *GameStatusOut;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void DrawContent(QPainter *painter, QPaintEvent *event);
    void OnClick(int button, int grid_x, int grid_y);
};

//! [0]

#endif
