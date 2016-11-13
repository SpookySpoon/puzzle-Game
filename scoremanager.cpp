#include <qmessagebox.h>
#include "scoremanager.h"
#include <qfile.h>
#include <qtextstream.h>
#include <qstring.h>
scoreManager::scoreManager(){}
scoreManager::~scoreManager(){}

QList<int> scoreManager::getScore()
{
    QList<int> results;
    QFile tempFile("MyPuzzleScore.txt");
    QString raw="";
    tempFile.open(QIODevice::ReadOnly);
    QTextStream txtMngr (&tempFile);
    raw = txtMngr.readLine();
    tempFile.close();
    QStringList cutRaw = raw.split("[::]",QString::SkipEmptyParts);
    if(cutRaw.count()<5||raw=="")
    {
        results<<-1;
    }
    else
    {
        for(auto i:cutRaw)
        {
            results<<i.toInt();
        }
    }
    return results;
}

void scoreManager::updateScore(int& time, int& moves)
{
    QList<int> currentRecords=scoreManager::getScore();
    int bestTime, bestScore, bestTimeComb, bestScoreComb, gamesPlayed;
    if(currentRecords.at(0)>0)
    {
        bestTime=currentRecords.takeFirst();
        bestScore=currentRecords.takeFirst();
        bestTimeComb=currentRecords.takeFirst();
        bestScoreComb=currentRecords.takeFirst();
        gamesPlayed=currentRecords.takeFirst();
        if(bestTime>time)
        {
            bestTime=time;
        }
        if(bestScore>moves)
        {
            bestScore=moves;
        }
        if((bestTimeComb>time)&&(bestScoreComb>moves))
        {
            bestTimeComb=time;
            bestScoreComb=moves;
        }
        gamesPlayed++;
    }
    else
    {
        bestTime=time;
        bestScore=moves;
        bestTimeComb=time;
        bestScoreComb=moves;
        gamesPlayed=1;
    }
    QString sendResults =QString("%1[::]%2[::]%3[::]%4[::]%5").
            arg(bestTime).arg(bestScore).arg(bestTimeComb).arg(bestScoreComb).arg(gamesPlayed);
    QFile tempFile("MyPuzzleScore.txt");
    tempFile.open(QIODevice::Truncate|QIODevice::WriteOnly);
    QTextStream outStream(&tempFile);
    outStream<<sendResults<<endl;
    tempFile.close();
}

void scoreManager::resetScore()
{
    QString sendResults="";
    QFile tempFile("MyPuzzleScore.txt");
    tempFile.open(QIODevice::Truncate|QIODevice::WriteOnly);
    QTextStream outStream(&tempFile);
    outStream<<sendResults<<endl;
    tempFile.close();
}
