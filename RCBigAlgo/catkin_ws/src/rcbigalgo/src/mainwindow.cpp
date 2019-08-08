#include "ui_mainwindow.h"

#include "../include/mainwindow.hpp"
#include "../include/action_executer.h"

#include <ros/ros.h>
#include <rcbigcar/board.h>

////////////////////////GAME/////////////////////////
enum GameUIStatus {
    GameStatus_Place_Red,
    GameStatus_Place_Blue,

    GameStatus_Move_Red,
    GameStatus_Move_Blue
};

QString GameStatusString[4] = {
    "START RED",
    "START BLUE",
    "RED",
    "BLUE"
};

Game::Board GlobalBoard;
GameUIStatus UIStatus;

int TimeLeft[2], MoveLeft[2];
int TurnCount = 0;

void ResetTurn() {
    TurnCount++;

    //time & step
    TimeLeft[0] = TimeLeft[1] = Game::GAME_TOTAL_MS;
    MoveLeft[0] = MoveLeft[1] = Game::GAME_TOTAL_STEP;
}

void ResetGame() {
    TurnCount = 0;

    //board
    memset(&GlobalBoard, 0, sizeof(GlobalBoard));

    GlobalBoard.ball[0] = GlobalBoard.ball[1] = 20;
    GlobalBoard.cup[0]  = GlobalBoard.cup[1]  = 10;

    //turn
    ResetTurn();

    //ui
    UIStatus = GameStatus_Place_Red;
}
///////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ResetGame();

    ui->setupUi(this);

    canvas = new VisualWidget(this);
    canvas->GameStatusOut = ui->tGameStatus;

    ui->paintLayout->addWidget(canvas, 0);
}

MainWindow::~MainWindow()
{
    delete canvas;
    delete ui;
}

///////////////////////////Visual Widget///////////////////////////
VisualWidget::VisualWidget(QWidget *parent)
    : QWidget(parent)
{
}

void VisualWidget::GameUpdate(void) {
    //draw graph
    this->update();

    bool who = (UIStatus == GameStatus_Move_Red) ? 0 : 1;

    //draw text
    this->GameStatusOut->setPlainText(
        QString("Turn No   = ") + QString::number(TurnCount) + QString("\n") +

        QString("Time Left = ") + QString::number(TimeLeft[who] / 1000) + QString("\n") + 
        QString("Move Left = ") + QString::number(MoveLeft[who]) + QString("\n") + 

        QString("Ball Left = ") + QString::number(GlobalBoard.ball[who]) + QString("\n") + 
        QString("Cup Left  = ") + QString::number(GlobalBoard.cup [who]) + QString("\n") +

        QString("Score Red = ")  + QString::number(Game::OP::get_score(GlobalBoard, 0)) + QString("\n") +
        QString("Score Blue = ") + QString::number(Game::OP::get_score(GlobalBoard, 1)) + QString("\n")
    );
}

void VisualWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    DrawContent(&painter, event);
    painter.end();
}

void VisualWidget::mousePressEvent(QMouseEvent *event) {
    QSize size = this->size();
    int grid_size = std::min(size.width(), size.height()) / std::max(Game::MAX_X, Game::MAX_Y);

    //draw grids
    for(int x = 0; x < Game::MAX_X; x++) for(int y = 0; y < Game::MAX_Y; y++) {
        QRect grid_cell = QRect(QPoint(y * grid_size, x * grid_size), QSize(grid_size, grid_size));

        if(grid_cell.contains(event->x(), event->y())) {
            //located click point
            OnClick(event->button(), x, y);

            return;
        }
    }
}

void VisualWidget::DrawContent(QPainter *painter, QPaintEvent *event) {
    //fill bg
    painter->fillRect(event->rect(), QBrush(Qt::white));

    //draw status
    int status_margins = std::min(this->size().width(), this->size().height()) * 0.1;
    painter->setPen  (QPen(Qt::black));

    painter->drawText(this->rect().adjusted(status_margins, status_margins, -status_margins, -status_margins).bottomLeft(),
                      GameStatusString[UIStatus]);

    //draw score
    painter->setPen  (QPen(Qt::black));
    painter->drawText(this->rect().adjusted(status_margins, status_margins, -status_margins, -status_margins).bottomRight(),
                      ((GlobalBoard.delta_points > 0) ? QString("RED") : QString("BLUE")) + QString(": ") + QString::number(std::abs(GlobalBoard.delta_points))); 

    /*
    //draw eval
    int eval_margins = std::min(this->size().width(), this->size().height()) * 0.3;
    painter->setPen  (QPen(Qt::black));
    painter->drawText(this->rect().marginsRemoved(QMargins(eval_margins, eval_margins, eval_margins, eval_margins)).bottomRight(),
                      QString("Red = ")    + QString::number(Game::Search::evaluate(0, GlobalBoard, 0, 0))
                    + QString("   Blue = ") + QString::number(Game::Search::evaluate(1, GlobalBoard, 0, 0))
    ); */

    //draw board
    QSize size = this->size();
    int grid_size = std::min(size.width(), size.height()) / std::max(Game::MAX_X, Game::MAX_Y);
    int grid_inner_margins = 0.2 * grid_size;

    //draw grids
    for(int x = 0; x < Game::MAX_X; x++) for(int y = 0; y < Game::MAX_Y; y++) {
        QRect grid_cell = QRect(QPoint(y * grid_size, x * grid_size), QSize(grid_size, grid_size));
        QRect grid_inner = grid_cell.adjusted(grid_inner_margins, grid_inner_margins, -grid_inner_margins, -grid_inner_margins);

        //draw cell
        painter->setBrush(QBrush(Qt::white));
        painter->setPen  (QPen(Qt::gray));
        painter->drawRect(grid_cell);

        //draw state
        int cell_state = GlobalBoard.board_state(x, y);
        painter->setPen  (Qt::NoPen);

        QColor cell_color = (cell_state == 0) ? Qt::red : Qt::blue;
        if(Game::get_bit(GlobalBoard.is_strong, x, y)) cell_color.setAlpha(128);
        else cell_color.setAlpha(64);

        if(cell_state != -1) {
            painter->setBrush(QBrush(cell_color));
            painter->setPen  (Qt::NoPen);
            painter->drawRect(grid_inner);
        }

        //draw castle
        if(Game::CASTLE_ID[x][y] != -1) {
            int castle_delta = GlobalBoard.castle[ Game::CASTLE_ID[x][y] ];

            if(castle_delta > 0) painter->setPen(QPen(Qt::red));
            else if(castle_delta < 0) painter->setPen(QPen(Qt::blue));
            else painter->setPen(QPen(Qt::black));

            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(grid_inner);
            painter->drawText(grid_inner.center(), QString::number( std::abs(castle_delta) ));
        }

        //draw placing position
        for(int place_who = 0; place_who < 2; place_who++) {
            if(Game::CASTLE_PLACE_ID[place_who][x][y] != -1) {
                if(place_who == 0) painter->setPen(Qt::red);
                else painter->setPen(Qt::blue);

                painter->drawLine(grid_cell.topLeft(), grid_cell.bottomRight());
            }
        }

        //draw bot
        for(int robot_id = 0; robot_id < 2; robot_id++) {
            if(COORD_SAME(x, y, GlobalBoard.position[robot_id][0], GlobalBoard.position[robot_id][1])) {
                if(robot_id == 0) painter->setBrush(QBrush(Qt::darkRed));
                else painter->setBrush(QBrush(Qt::darkBlue));

                painter->setPen  (Qt::NoPen);
                painter->drawEllipse(grid_inner);
            }
        }
    }
}

void VisualWidget::OnClick(int button, int grid_x, int grid_y) {
    bool Update = false;

    switch(UIStatus) {
        case GameStatus_Place_Red:
            if(button == Qt::LeftButton) {
                GlobalBoard.position[0][0] = grid_x;
                GlobalBoard.position[0][1] = grid_y;

                UIStatus = GameStatus_Place_Blue;
                Update   = true;
            }
        break;

        case GameStatus_Place_Blue:
            if(button == Qt::LeftButton) {
                GlobalBoard.position[1][0] = grid_x;
                GlobalBoard.position[1][1] = grid_y;

                UIStatus = GameStatus_Move_Red;
                Update   = true;
            }
        break;

        case GameStatus_Move_Red:
        case GameStatus_Move_Blue:
            bool who = (UIStatus == GameStatus_Move_Red) ? 0 : 1;

            if(button == Qt::LeftButton) {
                int x = GlobalBoard.position[who][0];
                int y = GlobalBoard.position[who][1];

                //get dir
                int dir = -1;
                if(x == grid_x) {
                    if(y > grid_y) dir = 3;
                    else           dir = 1;
                }
                else if(y == grid_y) {
                    if(x > grid_x) dir = 2;
                    else           dir = 0;
                }

                if(dir != -1) {
                    if((TimeLeft[who] >= Game::ACTION_MOVE_MS) && (MoveLeft[who] >= 1)) {
                        if(Game::OP::can_move(GlobalBoard, who, dir)) {
                            Game::OP::move(GlobalBoard, who, dir);

                            TimeLeft[who] -= Game::ACTION_MOVE_MS;
                            MoveLeft[who] -= 1;
                        }
                    }
                }
            }
            else if(button == Qt::RightButton) {
                if(TimeLeft[who] >= Game::ACTION_OCCUPY_MS) {
                    if(Game::OP::can_occupy(GlobalBoard, who)) {
                        Game::OP::occupy(GlobalBoard, who);

                        TimeLeft[who] -= Game::ACTION_OCCUPY_MS;
                    }
                }
            }

            Update   = true;
        break;
    }

    if(Update) GameUpdate();
}

void MainWindow::on_pbPlaceBall_clicked() {
    bool who = (UIStatus == GameStatus_Move_Red) ? 0 : 1;

    if((TimeLeft[who] >= Game::ACTION_PLACE_BALL_MS) && (GlobalBoard.ball[who])) {
        if(Game::OP::can_place(GlobalBoard, who)) {
            Game::OP::place(GlobalBoard, who, 1);

            GlobalBoard.ball[who]--;
            TimeLeft[who] -= Game::ACTION_PLACE_BALL_MS;
        }
    }

    canvas->GameUpdate();
}

void MainWindow::on_pbPlaceCup_clicked() {
    bool who = (UIStatus == GameStatus_Move_Red) ? 0 : 1;

    if((TimeLeft[who] >= Game::ACTION_PLACE_CUP_MS) && (GlobalBoard.cup[who])) {
        if(Game::OP::can_place(GlobalBoard, who)) {
            Game::OP::place(GlobalBoard, who, 4);

            GlobalBoard.cup[who]--;
            TimeLeft[who] -= Game::ACTION_PLACE_CUP_MS;
        }
    }

    canvas->GameUpdate();
}

////////////////////////////////////////////////////////////////////

void MainWindow::on_pEndTurn_clicked() {
    if(UIStatus == GameStatus_Move_Red)
        UIStatus = GameStatus_Move_Blue;
    else if(UIStatus == GameStatus_Move_Blue)
        UIStatus = GameStatus_Move_Red;

    ResetTurn();
    canvas->GameUpdate();
}

void MainWindow::on_pbSearch_clicked()
{
    for(int i = 0; i < 10; i++) {
        bool who = (UIStatus == GameStatus_Move_Red) ? 0 : 1;

        std::vector<uint8_t> strategies = Game::Search::search(who, GlobalBoard, MoveLeft[who], TimeLeft[who], 1000);

        for(int i = 0; i < strategies.size(); i++) {
            uint8_t st = strategies[i];

            if(st >= 0 && st <= 3) {
                Game::OP::move(GlobalBoard, who, st);

                TimeLeft[who] -= Game::ACTION_MOVE_MS;
                MoveLeft[who] -= 1;
            }
            else if(st == 4) {
                Game::OP::occupy(GlobalBoard, who);

                TimeLeft[who] -= Game::ACTION_OCCUPY_MS;
            }
            else if(st == 5) {
                Game::OP::place(GlobalBoard, who, 1);
                GlobalBoard.ball[who]--;

                TimeLeft[who] -= Game::ACTION_PLACE_BALL_MS;
            }
            else if(st == 6) {
                Game::OP::place(GlobalBoard, who, 4);
                GlobalBoard.cup[who]--;

                TimeLeft[who] -= Game::ACTION_PLACE_CUP_MS;
            }

            canvas->GameUpdate();
        }
    }

    on_pEndTurn_clicked();
}
