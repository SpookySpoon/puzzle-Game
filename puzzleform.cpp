#include "puzzleform.h"
#include "ui_puzzleform.h"
#include <QMessageBox>
#include <qtextstream.h>
#include <qstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <QPushButton>
#include <QTime>
#include <scoremanager.h>
#include <congratswindow.h>

PuzzleForm::PuzzleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PuzzleForm)
{
    ui->setupUi(this);
     for(int i =0;i<ui->gridLayout->count();i++)
     {
        PuzzleForm::puzzlePieces<<qobject_cast<QPushButton*>(ui->gridLayout->itemAt(i)->widget());
        connect(ui->gridLayout->itemAt(i)->widget(), SIGNAL(clicked(bool)), this, SLOT(on_pushButton(bool)));
        buttonInitialOrder<<(i);
     }
     PuzzleForm::numberOfMoves=0;
     PuzzleForm::elapsedSeconds=0;
     PuzzleForm::timer.start(1000,this);
     this->on_buttonShuffle_clicked();
}

PuzzleForm::~PuzzleForm()
{
    delete ui;
}


void PuzzleForm::findVacant(QWidget* puzzleButt,QGridLayout* puzzleGLayout)
{
    int num=puzzleGLayout->indexOf(puzzleButt);
    if(num!=-1)
    {
        int colCount=puzzleGLayout->columnCount();
        int rowCount=puzzleGLayout->rowCount();
        int  row, column, rowSpan, columnSpan;
        bool mark=false;
        puzzleGLayout->getItemPosition(num, &row, &column, &rowSpan, &columnSpan);
        for (int switchVertical=0; switchVertical<=1;switchVertical++)
        {
            if(!mark)
            {
                for(int switchHotizon=-1;switchHotizon<=1;switchHotizon+=2)
                {
                    int tempCol = std::max(column+switchHotizon*switchVertical,0);
                    tempCol=std::min(tempCol,colCount-1);
                    int tempRow = std::max(row+switchHotizon*(1-switchVertical),0);
                    tempRow=std::min(tempRow,rowCount-1);
                    if(puzzleGLayout->itemAtPosition(tempRow,tempCol)==0)
                    {
                        puzzleGLayout->addWidget(puzzleButt,tempRow,tempCol,1,1/*,Qt::AlignCenter*/);
                        numberOfMoves++;
                        ui->labelMoves->setText(QString("Moves: %1").arg(numberOfMoves));
                        mark=true;
                        break;
                    }
                }
            }
            else
            {break;}
        }
    }
}

void PuzzleForm::reset()
{
    if(timer.isActive())
    {
        timer.stop();
    }
    numberOfMoves=0;
    elapsedSeconds=0;
    timer.start(1000,this);
    ui->lableElapsedTime->setText(QString("Time: %1").arg(QDateTime::fromTime_t(elapsedSeconds).toUTC().toString("hh:mm:ss")));
    ui->labelMoves->setText("Moves: 0");
}

void PuzzleForm::startOver()
{
    int member=15;
    for (int i_row=0;i_row<4;i_row++)
    {
        for (int i_col=0;i_col<4;i_col++)
        {
            if(member>=1)
            {
                ui->gridLayout->addWidget(puzzlePieces.at(buttonInitialOrder.at(15-member)),i_row,i_col,1,1/*,Qt::AlignCenter*/);
                member--;
            }
        }
    }
    countScore=false;
}

void PuzzleForm::newGame()
{
    on_buttonShuffle_clicked();
}


void PuzzleForm::checkResult()
{
    QStringList checkList;
    for(int i_row=1;i_row<5;i_row++)
    {
        for(int i_col=1;i_col<5;i_col++)
        {
            if(ui->gridLayout->itemAtPosition(i_row-1,i_col-1)!=0)
            {
                checkList<<ui->gridLayout->itemAtPosition(i_row-1,i_col-1)->widget()->objectName();
            }
            else
            {
                checkList<<"Empty Layout Item";
            }
        }
    }
    bool result=true;
    for(int i=0;i<puzzlePieces.count();i++)
    {
        if(puzzlePieces.at(i)->objectName()!=checkList.at(i))
        {
            result=false;
        }
    }
    if (result)
    {
        if(timer.isActive())
        {
            timer.stop();
        }
        CongratsWindow* conWindow=new CongratsWindow(0,numberOfMoves,elapsedSeconds,this, countScore);
        conWindow->exec();
    }
}

void PuzzleForm::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == timer.timerId())
    {
        ++elapsedSeconds;
        ui->lableElapsedTime->setText(QString("Time: %1").arg(QDateTime::fromTime_t(elapsedSeconds).toUTC().toString("hh:mm:ss")));
    }
}

void PuzzleForm::on_buttonQuit_clicked()
{
    this->close();
}

void PuzzleForm::on_buttonRestart_clicked()
{
    startOver();
    reset();
}

void PuzzleForm::on_pushButton(bool mark1)
{
    QString name = sender()->objectName();
    for (int i1=0;i1<ui->gridLayout->count();i1++)
    {
        QWidget* tempWidget = ui->gridLayout->itemAt(i1)->widget();
        if(tempWidget->objectName()==name)
        {
            PuzzleForm::findVacant(tempWidget,ui->gridLayout);
            checkResult();
            break;
        }
    }
}

void PuzzleForm::on_buttonShuffle_clicked()
{
    reset();
    QList<int*> randPick;
    buttonInitialOrder.clear();
    for (int i=0;i<15;i++)
    {
        randPick<<(new int(i));
    }
    srand (time(NULL));
    int member=15,iSecret;

    for (int i_row=0;i_row<4;i_row++)
    {
        for (int i_col=0;i_col<4;i_col++)
        {
            if(member>=1)
            {
                iSecret = rand() % member + 1;
                int* takenRandNum=randPick.takeAt(iSecret-1);
                ui->gridLayout->addWidget(puzzlePieces.at(*takenRandNum),i_row,i_col,1,1/*,Qt::AlignCenter*/);
                buttonInitialOrder<<*takenRandNum;
                delete takenRandNum;
                member--;
            }
        }
    }
    countScore=true;
}

void PuzzleForm::on_buttonCheat_clicked()
{
    int item=0;
    for(int i_row=0;i_row<4;i_row++)
    {
        for(int i_col=0;i_col<4;i_col++)
        {
            if(item<14)
            {
                ui->gridLayout->addWidget(puzzlePieces.at(item),i_row,i_col,1,1);
                item++;
            }
            else if(item=14)
            {
                ui->gridLayout->addWidget(puzzlePieces.at(item),3,3,1,1);
            }
        }
    }
}
