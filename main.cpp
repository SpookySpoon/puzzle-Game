#include "puzzleform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PuzzleForm w;
    w.show();
    return a.exec();
}
