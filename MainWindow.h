#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QTabWidget>
#include <QDialog>
#include <vector>
#include <QFormLayout>
#include <QSpinBox>
#include <QColorDialog>

#include "MyCanvas.h"

struct DialogTab2{
    QWidget *main_widget;
    QVBoxLayout *mainLay;
    QFormLayout *pc_lay;
    QSpinBox *pc_spin;
    QFormLayout *nbrockets_lay;
    QSpinBox *nbrockets_spin;
    QPushButton *validateButton;
    QColorDialog *ColorDiag;
    QPushButton *colorButton;

};

typedef DialogTab2 DialogTab;

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
    void manageRace();
    void sendRacesParam();
    void update_colors();

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
    
    QPushButton *clearObstacles;
    
    QPushButton *manage_races;
    
    //DIALOG for RACES
    void initRacesDialog();
    QDialog *races_dialog;
    QHBoxLayout *racesdialog_lay;
    QTabWidget *racesdialog_tabs; 
    std::vector<DialogTab> racesdialogtab_widgets;
};

#endif // MAINWINDOW_H
