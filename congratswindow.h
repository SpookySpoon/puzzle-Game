#pragma once
#include <QDialog>
#include "puzzleform.h"

namespace Ui {
class CongratsWindow;
}

class CongratsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CongratsWindow(const int moves =0, const int time=0, PuzzleForm* buddy=0, const bool countScore=true, QWidget* parent = 0);
    ~CongratsWindow();

private slots:
    void on_buttonQuit_clicked();
    void on_buttonTryAgain_clicked();
    void on_buttonResetHistory_clicked();
    void on_buttonNewGame_clicked();

private:
    void initNoScoreLables();
    void initLables(const int moves, const int time);
    Ui::CongratsWindow *ui;
    PuzzleForm* buddy;
};
