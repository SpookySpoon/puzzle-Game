#include <QDateTime>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "puzzleform.h"
#include "ui_puzzleform.h"
#include "scoremanager.h"
#include "congratswindow.h"





PuzzleForm::PuzzleForm(QWidget *parent) :
    QWidget(parent),ui(new Ui::PuzzleForm),numberOfMoves(0),elapsedSeconds(0), countScore(true)
{
    ui->setupUi(this);
    //В цикле ниже перебираются все кнопки пазла, включенные в gridLayout.
    //Они одновременно добавляются как ссылки в список кнопок "puzzlePieces",
    //а также от каждой кнопки берется сигнал, который будет запускать функцию "on_pushButton(bool mark=false)".
    //Потом просто при нажатии кнопки функция получит имя кнопки, найдет её в сетке пазла и применит к ней функцию поиска пустой ячейки "findVacant".
    //В этой функции я добавил булевый аргумент, потому что мне Qt не давал связать слот и сигнал с разными аргументами
    //Так что по сути аргумент слота это просто аппендикс.
     for(int i =0;i<ui->gridLayout->count();i++)
     {
        PuzzleForm::puzzlePieces<<qobject_cast<QPushButton*>(ui->gridLayout->itemAt(i)->widget());
        connect(ui->gridLayout->itemAt(i)->widget(), SIGNAL(clicked(bool)), this, SLOT(on_pushButton(bool)));
        buttonInitialOrder<<(i);
     }
     //Эта функция (по совместительству слот) перемешивает кнопки случайным образом и запускает таймер для подсчета времени на партию.
     on_buttonShuffle_clicked();
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

void PuzzleForm::sameGameAgain()
{
    startOver();
    reset();
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
        CongratsWindow* conWindow=new CongratsWindow(numberOfMoves,elapsedSeconds,this, countScore, 0);
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
    close();
}

void PuzzleForm::on_buttonRestart_clicked()
{
    sameGameAgain();
}

void PuzzleForm::on_pushButton(bool mark1)
{
    PuzzleForm::findVacant(qobject_cast<QWidget*>(sender()),ui->gridLayout);
    checkResult();
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
