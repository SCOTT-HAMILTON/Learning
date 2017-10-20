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
    QColorDialog *ColorDiag;
    QPushButton *colorButton;
    QHBoxLayout *buttonsLay; 
    QPushButton *validateButton;
    QPushButton *deleteButton;

};

using DialogTab = DialogTab2;

class DialogTabConnector : public QObject
{
    Q_OBJECT

public:
    DialogTabConnector(int index_tab, const QTabWidget *tab, QPushButton *sender, const std::vector<DialogTab> *widgets_tabs);
    void setIndex(int index);
    
public slots:
    void deleteTab();

signals:
    void tab_deleted(int);
    
private:
    int index;
    QTabWidget *tab;
    std::vector<DialogTab> *widgets_tabs;
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void changeGen(int);
    void sendGotoGen();
    void sendResetGen();
    void sendRestartGen();
    void sendMoveCible();
    void manageRace();
    void sendRacesParam();
    void sendCreateRace();
    void update_colors();
    void delete_DialogTabConnector(int);
    
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
    std::vector<DialogTabConnector*> racesdialogtab_connector;
};

#endif // MAINWINDOW_H
