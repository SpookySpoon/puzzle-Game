#include "congratswindow.h"
#include <ui_congratswindow.h>
#include <scoremanager.h>
#include <QDateTime>
#include <puzzleform.h>

CongratsWindow::CongratsWindow(QWidget *parent, int moves ,  int time, PuzzleForm* buddy, bool countScore) :
    QDialog(parent),
    ui(new Ui::CongratsWindow),buddy(buddy)
{
    ui->setupUi(this);
    if(countScore)
    {
        ui->lableCheers->setText("Congratulations, you've solved a broblem!!");
        scoreManager::updateScore(time,moves);
    }
    else
    {
        ui->lableCheers->setText("Nice practise, now go play it for real!");
    }
    CongratsWindow::initLables(moves, time);
}

CongratsWindow::~CongratsWindow()
{
    delete ui;
}

void CongratsWindow::initLables(const int& moves ,  const int& time)
{
    QList<int> lableData=scoreManager::getScore();
    ui->lableCurScoreMoves->setText(QString("Number of moves: %1").arg(moves));
    ui->lableCurScoreTime->setText(QString("Time: %1").arg(QDateTime::fromTime_t(time).toUTC().toString("hh:mm:ss")));
    if(lableData.first()>0)
    {
        ui->lableHistBTime->setText(QString("Your best time: %1")
                                    .arg(QDateTime::fromTime_t(lableData.takeFirst()).toUTC().toString("hh:mm:ss")));
        ui->lableHistBMoves->setText(QString("The least number of moves: %1").arg(lableData.takeFirst()));
        ui->lableHistComboTime->setText(QString("Time: %1")
                                    .arg(QDateTime::fromTime_t(lableData.takeFirst()).toUTC().toString("hh:mm:ss")));
        ui->lableHistComboMoves->setText(QString("Number of moves: %1").arg(lableData.takeFirst()));
    }
    else
    {
        CongratsWindow::initNoScoreLables();
    }
}

void CongratsWindow::initNoScoreLables()
{
    ui->lableHistBMoves->setText("The least number of moves: n/a");
    ui->lableHistBTime->setText("Your best time: n/a");
    ui->lableHistComboMoves->setText("Number of moves: n/a");
    ui->lableHistComboTime->setText("Time: n/a");
}

void CongratsWindow::on_buttonQuit_clicked()
{
    buddy->close();
    this->close();
}

void CongratsWindow::on_buttonTryAgain_clicked()
{
    buddy->startOver();
    buddy->reset();
    this->close();
}

void CongratsWindow::on_buttonResetHistory_clicked()
{
    scoreManager::resetScore();
    CongratsWindow::initNoScoreLables();
}

void CongratsWindow::on_buttonNewGame_clicked()
{
    buddy->newGame();
    this->close();

}
