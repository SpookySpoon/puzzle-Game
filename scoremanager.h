#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include <qstring.h>

class scoreManager
{
public:
    scoreManager();
    ~scoreManager();
    static void updateScore(int& time, int& moves);
    static QList<int> getScore();
    static void resetScore();
};

#endif // SCOREMANAGER_H
