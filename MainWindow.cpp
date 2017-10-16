#include "MainWindow.h"
#include <sstream>


#ifndef toSTR
#define toSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif //toSTR

MainWindow::MainWindow()
{
    setFixedSize({1065, 480});
    mainLay = new QHBoxLayout;

    canvas = new MyCanvas({840, 480});
    canvas->setFixedSize({840, 480});

    controlLay = new QVBoxLayout;

    genTxtLabel = new QLabel("Go to a higher generation : ");

    genLay = new  QVBoxLayout;

    genBox = new QSpinBox;
    genBox->setValue(canvas->getCurrentGeneration());
    std::cout <<"val : " << canvas->getCurrentGeneration() << std::endl;
    genBox->setMaximum(genBox->value()+10000);
    genBox->setMinimum(genBox->value());
    validGenButton = new QPushButton("Go to this generation");
    restartGenButton = new QPushButton("Reset generation");

    genLay->addWidget(genTxtLabel);
    genLay->addWidget(genBox);
    genLay->addWidget(validGenButton);
    genLay->addWidget(restartGenButton);

    box_gen = new QGroupBox("Generations");
    box_gen->setLayout(genLay);

    moveCibleText = new QPushButton("Change cible position");

    createObstacle = new QPushButton("Create rectangle obstacle");
    
    clearObstacles = new QPushButton("Clear obstacles");
    
    manage_races = new QPushButton("Manage races");
    
    
    controlLay->addWidget(box_gen, 20, Qt::AlignTop|Qt::AlignLeft);
    controlLay->addWidget(moveCibleText, 30, Qt::AlignTop|Qt::AlignLeft);
    controlLay->addWidget(createObstacle, 5, Qt::AlignTop|Qt::AlignLeft);
    controlLay->addWidget(clearObstacles, 50, Qt::AlignTop|Qt::AlignLeft);
    controlLay->addWidget(manage_races, 50, Qt::AlignTop|Qt::AlignLeft);
    mainLay->addLayout(controlLay);
    mainLay->addWidget(canvas);

    setLayout(mainLay);
    
    initRacesDialog();

    QObject::connect(canvas, SIGNAL(genChanged(int)), this, SLOT(changeGen(int)));
    QObject::connect(validGenButton, SIGNAL(clicked(bool)), this, SLOT(sendGotoGen()));
    QObject::connect(restartGenButton, SIGNAL(clicked(bool)), this, SLOT(sendRestartGen()));
    QObject::connect(moveCibleText, SIGNAL(clicked(bool)), this, SLOT(sendMoveCible()));
    QObject::connect(createObstacle, SIGNAL(clicked(bool)), canvas, SLOT(addRectObst()));
    QObject::connect(clearObstacles, SIGNAL(clicked(bool)), canvas, SLOT(deleteObstacles()));
    QObject::connect(manage_races, SIGNAL(clicked(bool)), this, SLOT(manageRace()));
}

void MainWindow::changeGen(int val){
    genBox->setMaximum(val+10000);
    genBox->setMinimum(genBox->value());
}

void MainWindow::sendGotoGen(){
    canvas->gotoGen(genBox->value());
}

void MainWindow::sendRestartGen(){
    canvas->resetGen();
}

void MainWindow::sendMoveCible(){
    canvas->moveCible();
}

void MainWindow::initRacesDialog(){
    races_dialog = new QDialog;
    std::vector<Race> *races = canvas->getPopRaces();
    racesdialog_lay = new QHBoxLayout;
    racesdialog_tabs = new QTabWidget;
    for (std::size_t i = 0; i < races->size(); i++){
        DialogTab tab_widget;
        tab_widget.mainLay = new QVBoxLayout;
        tab_widget.main_widget = new QWidget;
        tab_widget.nbrockets_lay = new QFormLayout;
        tab_widget.nbrockets_spin = new QSpinBox;
        tab_widget.pc_lay = new QFormLayout;
        tab_widget.pc_spin = new QSpinBox;
        tab_widget.validateButton = new QPushButton("Apply changes");
        tab_widget.pc_spin->setValue((*(races))[i].raceData.pc_obey);
        tab_widget.pc_spin->setMaximum(100);
        
        tab_widget.nbrockets_spin->setMaximum(1000000);
        tab_widget.nbrockets_spin->setValue((*(races))[i].rockets.size());
        
        tab_widget.pc_lay->addRow("Pourcentage of obey : ", tab_widget.pc_spin);
        tab_widget.nbrockets_lay->addRow("Number of rockets : ", tab_widget.nbrockets_spin);
        
        tab_widget.colorButton = new QPushButton(tab_widget.main_widget);
        
        QPalette palette;
        QColor color((*(races))[i].raceData.pc_obey*2.55f, 255-((float)(*(races))[i].raceData.pc_obey*2.55), 255);
        
        tab_widget.ColorDiag = new QColorDialog(tab_widget.main_widget);
        tab_widget.ColorDiag->setCurrentColor(color);
        
        palette.setColor(QPalette::Button, tab_widget.ColorDiag->currentColor());
        tab_widget.colorButton->setPalette(palette);
        tab_widget.colorButton->setMaximumSize(25, 25);
        tab_widget.colorButton->setObjectName("color_button");
        
        std::cout << "obj" << tab_widget.ColorDiag->parent()->findChild<QPushButton*>("color_button")->objectName().toStdString() << std::endl;
        
        tab_widget.mainLay->addLayout(tab_widget.pc_lay);
        tab_widget.mainLay->addLayout(tab_widget.nbrockets_lay);
        tab_widget.mainLay->addWidget(tab_widget.colorButton);
        tab_widget.mainLay->addWidget(tab_widget.validateButton);
        
        tab_widget.main_widget->setLayout(tab_widget.mainLay);
        
        racesdialog_tabs->addTab(tab_widget.main_widget, QString::fromStdString(std::string("Race "+toSTR(i))));
        
        racesdialogtab_widgets.push_back(tab_widget);
        
        QObject::connect(tab_widget.validateButton, SIGNAL(clicked(bool)), this, SLOT(sendRacesParam()));
        QObject::connect(tab_widget.colorButton, SIGNAL(clicked(bool)), tab_widget.ColorDiag, SLOT(open()));
        QObject::connect(tab_widget.ColorDiag, SIGNAL(colorSelected(QColor)), this, SLOT(update_colors()));
    }
    
    racesdialog_lay->addWidget(racesdialog_tabs);
    
    races_dialog->setLayout(racesdialog_lay);
}

void MainWindow::manageRace(){
    races_dialog->exec();
}

void MainWindow::sendRacesParam(){
    std::vector<RaceData> params;
    for (std::size_t i = 0; i < racesdialogtab_widgets.size(); i++){
        RaceData race;
        race.pc_obey = racesdialogtab_widgets[i].pc_spin->value();
        
        race.success = 0;
        race.nb_rockets = racesdialogtab_widgets[i].nbrockets_spin->value();
        QColor c = racesdialogtab_widgets[i].ColorDiag->currentColor();
        race.color = sf::Color(c.red(), c.green(), c.blue());
        params.push_back(race);
    }
    
    canvas->setPopRaces(params);
    
    sendRestartGen();
}

void MainWindow::update_colors(){
    std::cout << "yah" << std::endl;
    
    for (std::size_t i = 0; i < racesdialogtab_widgets.size(); i++){
        QPalette palette;
        palette.setColor(QPalette::Button, racesdialogtab_widgets[i].ColorDiag->currentColor());
        racesdialogtab_widgets[i].colorButton->setPalette(palette);
    }
}


