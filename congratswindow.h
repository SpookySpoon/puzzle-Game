#ifndef CONGRATSWINDOW_H
#define CONGRATSWINDOW_H

#include <QDialog>
#include <puzzleform.h>

namespace Ui {
class CongratsWindow;
}

class CongratsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CongratsWindow(QWidget* parent = 0, int moves =0, int time=0, PuzzleForm* buddy=0,bool countScore=true);
    ~CongratsWindow();

private slots:
    void on_buttonQuit_clicked();
    void on_buttonTryAgain_clicked();
    void on_buttonResetHistory_clicked();
    void on_buttonNewGame_clicked();

private:
    void initNoScoreLables();
    void initLables(const int& moves ,  const int& time);
    Ui::CongratsWindow *ui;
    PuzzleForm* buddy;
};

#endif // CONGRATSWINDOW_H
