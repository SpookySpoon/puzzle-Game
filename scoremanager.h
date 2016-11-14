#pragma once
#include <QList>

class scoreManager
{
public:
    static void updateScore(const int time, const int moves);
    static QList<int> getScore();
    static void resetScore();
};
