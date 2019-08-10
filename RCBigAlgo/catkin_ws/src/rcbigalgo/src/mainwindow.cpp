#include "ui_mainwindow.h"

#include "../include/mainwindow.hpp"
#include "../include/action_executer.h"

#include <ros/ros.h>
#include <QTimer>
#include <ros/ros.h>

////////////////////////GAME/////////////////////////

bool JudgeBoardInitialized = false;
rcbigcar::board JudgeBoard;

int LastTurnWho = -1;

void InitGlobalBoard() {
    //board
    memset(&GlobalBoard, 0, sizeof(GlobalBoard));

    GlobalBoard.ball[0] = GlobalBoard.ball[1] = 20;
    GlobalBoard.cup[0]  = GlobalBoard.cup[1]  = 10;

    GlobalBoard.position[0][0] = 0;
    GlobalBoard.position[0][1] = 0;

    GlobalBoard.position[1][0] = 0;
    GlobalBoard.position[1][1] = 8;
}

void LoadJudgeBoard() {
    GlobalBoard.position[0][0] = JudgeBoard.robot_x[0];
    GlobalBoard.position[0][1] = JudgeBoard.robot_y[0];

    GlobalBoard.position[1][0] = JudgeBoard.robot_x[1];
    GlobalBoard.position[1][1] = JudgeBoard.robot_y[1];

    for(int i = 0; i < 7; i++) {
        GlobalBoard.castle[i] = JudgeBoard.castle[i];
    }

    for(int x = 0; x < 7; x++) for(int y = 0; y < 9; y++) {
        int8_t status = JudgeBoard.cell_status[x * 9 + y];

        Game::set_bit(GlobalBoard.is_occupy, x, y, status != -1);
        Game::set_bit(GlobalBoard.who,       x, y, (status == 0) ? 0 : 1);
    }

    Game::CC::recalc_strong(&GlobalBoard);
}

void MainWindow::UpdateStrategy() {
    if(!JudgeBoardInitialized) return;

    //load judge to global board
    LoadJudgeBoard();

    int CurTurnWho = JudgeBoard.team;
    if((JudgeBoard.time_left == 0) || (JudgeBoard.move_left == 0)) CurTurnWho = !CurTurnWho;

    //If switched and turn on our team
    if(CurTurnWho != LastTurnWho) {
        if(CurTurnWho == WhoAmI) {
            //recalc strategy
            std::vector<uint8_t> all_strategies;

            Game::Board temp_board = GlobalBoard;

            int temp_time_left = JudgeBoard.time_left * 1000;
            int temp_move_left = JudgeBoard.move_left;

            //pre-search
            if(JudgeBoard.team != CurTurnWho) {
                temp_time_left = Game::GAME_TOTAL_MS;
                temp_move_left = Game::GAME_TOTAL_STEP;
            }

            for(int i = 0; i < 3; i++) {
                std::vector<uint8_t> strategy_piece = Game::Search::search(WhoAmI, temp_board, temp_move_left, temp_time_left, 1000);

                for(int i = 0; i < strategy_piece.size(); i++) {
                    uint8_t st = strategy_piece[i];

                    if(st >= 0 && st <= 3) {
                        Game::OP::move(temp_board, WhoAmI, st);

                        temp_move_left--;
                        temp_time_left -= Game::ACTION_MOVE_MS;
                    }
                    else if(st == 4) {
                        Game::OP::occupy(temp_board, WhoAmI);

                        temp_time_left -= Game::ACTION_OCCUPY_MS;
                    }
                    else if(st == 5) {
                        Game::OP::place(temp_board, WhoAmI, 1);

                        temp_time_left -= Game::ACTION_PLACE_BALL_MS;

                        temp_board.ball[WhoAmI]--;
                    }
                    else if(st == 6) {
                        Game::OP::place(temp_board, WhoAmI, 6);

                        temp_time_left -= Game::ACTION_PLACE_CUP_MS;

                        temp_board.cup[WhoAmI]--;
                    }

                    //push to all strategy list
                    all_strategies.push_back(st);
                }
            }

            //apply strategy
            ActionExecuter::LoadActionList( GlobalPlanner::GetActions(WhoAmI, GlobalBoard, all_strategies) );
        }

        LastTurnWho = CurTurnWho;
    }
}
///////////////////////////////////////////////////////

ros::Subscriber board_sub;

void MainWindow::BoardCallback(const rcbigcar::board::ConstPtr &board) {
    JudgeBoard = *board;

    JudgeBoardInitialized = true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ros::NodeHandle nh;

    InitGlobalBoard();

    //Setup Auto
    MechanicalExecuter::init();
    ActionExecuter::init();

    //Setup Comm
    board_sub = nh.subscribe<rcbigcar::board>("board", 100, &MainWindow::BoardCallback, this);

    //Setup Tick Timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000 / 20);

    //Setup UI
    ui->setupUi(this);

    canvas = new VisualWidget(this);
    canvas->GameStatusOut = ui->tGameStatus;

    ui->paintLayout->addWidget(canvas, 0);

    //Setup UI Update Timer
    ui_update_timer = new QTimer(this);
    connect(ui_update_timer, SIGNAL(timeout()), canvas, SLOT(GameUpdate()));
    ui_update_timer->start(1000 / 10);
}

MainWindow::~MainWindow()
{
    delete timer;
    delete canvas;
    delete ui;
}

void MainWindow::update() {
    MechanicalExecuter::update();
    ActionExecuter::update();

    UpdateStrategy();
}

///////////////////////////Visual Widget///////////////////////////
VisualWidget::VisualWidget(QWidget *parent)
    : QWidget(parent)
{
}

void VisualWidget::GameUpdate(void) {
    //draw graph
    this->update();

    //draw text
    if(JudgeBoardInitialized) {
      this->GameStatusOut->setPlainText(
          QString("Move   = ") +    (( JudgeBoard.team == 0 ) ? QString("Red") : QString("Blue")) + QString("\n") +

          QString("Time Left = ") + QString::number( JudgeBoard.time_left ) + QString("\n") +
          QString("Move Left = ") + QString::number( JudgeBoard.move_left ) + QString("\n") +

          QString("Ball Left = ") + QString::number(  GlobalBoard.ball[WhoAmI] ) + QString("\n") +
          QString("Cup Left  = ") + QString::number(  GlobalBoard.cup [WhoAmI] ) + QString("\n") +

          QString("Score Red = ")  + QString::number( JudgeBoard.score[0] ) + QString("\n") +
          QString("Score Blue = ") + QString::number( JudgeBoard.score[1] ) + QString("\n")
      );
    }
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

}

////////////////////////////////////////////////////////////////////

void MainWindow::on_pWhoAmI_clicked()
{
    WhoAmI = !WhoAmI;
    ui->pWhoAmI->setText( (WhoAmI == 0) ? "RED" : "BLUE" );
}

void MainWindow::on_pTeamOk_clicked()
{
    ui->pWhoAmI->setDisabled(true);
    ui->pTeamOk->setDisabled(true);
    ui->lbSelTeam->setVisible(false);
}
