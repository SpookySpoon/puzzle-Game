#include <QApplication>
#include "puzzleform.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzleForm w;
    w.show();
    return a.exec();
}
