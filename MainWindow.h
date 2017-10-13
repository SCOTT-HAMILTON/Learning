#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include "MyCanvas.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void changeGen(int);
    void sendGotoGen();
    void sendRestartGen();
    void sendMoveCible();


private:
    QHBoxLayout *mainLay;
    MyCanvas *canvas;

    QVBoxLayout *controlLay;

    QVBoxLayout *genLay;
    QLabel *genTxtLabel;
    QSpinBox *genBox;
    QPushButton *validGenButton;

    QPushButton *restartGenButton;
    QGroupBox *box_gen;

    QPushButton *moveCibleText;

    QPushButton *createObstacle;
};

#endif // MAINWINDOW_H
