#include "MainWindow.h"
#include <sstream>
#include <QApplication>


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
    
    manage_races = new QPushButton("Manage breeds");
    
    
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
    races_dialog->setMinimumSize(races->size()*68+73, 200);
    racesdialog_lay = new QHBoxLayout;
    racesdialog_tabs = new QTabWidget;
    for (std::size_t i = 0; i < races->size()+1; i++){
        DialogTab tab_widget;
        tab_widget.mainLay = new QVBoxLayout;
        tab_widget.main_widget = new QWidget;
        tab_widget.nbrockets_lay = new QFormLayout;
        tab_widget.nbrockets_spin = new QSpinBox;
        tab_widget.pc_lay = new QFormLayout;
        tab_widget.pc_spin = new QSpinBox;
        if (i == races->size()){
            tab_widget.validateButton = new QPushButton("Create race");
            tab_widget.pc_spin->setValue(50);
        }
        else {
            tab_widget.validateButton = new QPushButton("Apply changes");
            tab_widget.pc_spin->setValue((*(races))[i].raceData.pc_obey);
            tab_widget.buttonsLay = new QHBoxLayout;
            QIcon icondel(QApplication::applicationDirPath()+"/bin.png");
            tab_widget.deleteButton = new QPushButton(icondel, "");
            tab_widget.deleteButton->setFixedSize(30, 30);
            tab_widget.buttonsLay->addWidget(tab_widget.validateButton);
            tab_widget.buttonsLay->addWidget(tab_widget.deleteButton);
        }
        tab_widget.pc_spin->setMaximum(100);
        
        tab_widget.nbrockets_spin->setMaximum(1000000);
        if (i == races->size())tab_widget.nbrockets_spin->setValue(100);
        else tab_widget.nbrockets_spin->setValue((*(races))[i].rockets.size());
        
        tab_widget.pc_lay->addRow("Percentage of obedience : ", tab_widget.pc_spin);
        tab_widget.nbrockets_lay->addRow("Number of rockets : ", tab_widget.nbrockets_spin);
        
        tab_widget.colorButton = new QPushButton(tab_widget.main_widget);
        
        QPalette palette;
        QColor color;
        if (i == races->size())color = QColor(255, 255, 255);
        else color = QColor((float)(*(races))[i].raceData.pc_obey*2.55f, 255-((float)(*(races))[i].raceData.pc_obey*2.55), 255);
        
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
        if (i == races->size())tab_widget.mainLay->addWidget(tab_widget.validateButton);
        else tab_widget.mainLay->addLayout(tab_widget.buttonsLay);
        
        tab_widget.main_widget->setLayout(tab_widget.mainLay);
        
        if (i == races->size()) racesdialog_tabs->addTab(tab_widget.main_widget, "+");
        else racesdialog_tabs->addTab(tab_widget.main_widget, QString::fromStdString(std::string("Breed "+toSTR(i))));
        
        racesdialogtab_widgets.push_back(tab_widget);
        
        if (i == races->size())QObject::connect(tab_widget.validateButton, SIGNAL(clicked(bool)), this, SLOT(sendCreateRace()));
        else QObject::connect(tab_widget.validateButton, SIGNAL(clicked(bool)), this, SLOT(sendRacesParam()));
        QObject::connect(tab_widget.colorButton, SIGNAL(clicked(bool)), tab_widget.ColorDiag, SLOT(open()));
        QObject::connect(tab_widget.ColorDiag, SIGNAL(colorSelected(QColor)), this, SLOT(update_colors()));
    }
    
    racesdialog_lay->addWidget(racesdialog_tabs);
    
    races_dialog->setLayout(racesdialog_lay);
    std::cout << "w : " << racesdialog_tabs->size().width() << std::endl;
    std::cout << "h : " << racesdialog_tabs->size().height() << std::endl;
}

void MainWindow::manageRace(){
    races_dialog->exec();
}

void MainWindow::sendRacesParam(){
    std::vector<RaceData> params;
    for (std::size_t i = 0; i < racesdialogtab_widgets.size()-1; i++){
        RaceData race;
        race.pc_obey = racesdialogtab_widgets[i].pc_spin->value();
        race.success = 0;
        race.nb_rockets = racesdialogtab_widgets[i].nbrockets_spin->value();
        QColor c = racesdialogtab_widgets[i].ColorDiag->currentColor();
        race.color = sf::Color(c.red(), c.green(), c.blue());
        params.push_back(race);
    }
    std::cout << "lol" << std::endl;
    canvas->setPopRaces(params);
    std::cout << "end" << std::endl;
    sendRestartGen();
    
}

void MainWindow::sendCreateRace(){
    races_dialog->setMinimumSize(racesdialog_tabs->count()*68+73, 200);
    racesdialog_tabs->setTabText(racesdialog_tabs->count()-1, QString::fromStdString(std::string("Breed "+toSTR(racesdialog_tabs->count()-1))));
    
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].validateButton->setText("Apply changes");
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].buttonsLay = new QHBoxLayout;
    QIcon icondel(QApplication::applicationDirPath()+"/bin.png");
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].deleteButton = new QPushButton(icondel, "");
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].deleteButton->setFixedSize(30, 30);
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].buttonsLay->addWidget(racesdialogtab_widgets[racesdialogtab_widgets.size()-1].validateButton);
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].buttonsLay->addWidget(racesdialogtab_widgets[racesdialogtab_widgets.size()-1].deleteButton);
    
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].mainLay->removeWidget(racesdialogtab_widgets[racesdialogtab_widgets.size()-1].validateButton);
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].mainLay->addLayout(racesdialogtab_widgets[racesdialogtab_widgets.size()-1].buttonsLay);
    
    racesdialogtab_widgets[racesdialogtab_widgets.size()-1].validateButton->disconnect();
    QObject::connect(racesdialogtab_widgets[racesdialogtab_widgets.size()-1].validateButton, SIGNAL(clicked(bool)), this, SLOT(sendRacesParam()));
    
    DialogTab tab_widget;
    tab_widget.mainLay = new QVBoxLayout;
    tab_widget.main_widget = new QWidget;
    tab_widget.nbrockets_lay = new QFormLayout;
    tab_widget.nbrockets_spin = new QSpinBox;
    tab_widget.pc_lay = new QFormLayout;
    tab_widget.pc_spin = new QSpinBox;
    tab_widget.validateButton = new QPushButton("Create race");
    tab_widget.pc_spin->setValue(50);
    tab_widget.pc_spin->setMaximum(100);
    
    tab_widget.nbrockets_spin->setMaximum(1000000);
    tab_widget.nbrockets_spin->setValue(100);
    
    tab_widget.pc_lay->addRow("Percentage of obedience : ", tab_widget.pc_spin);
    tab_widget.nbrockets_lay->addRow("Number of rockets : ", tab_widget.nbrockets_spin);
    
    tab_widget.colorButton = new QPushButton(tab_widget.main_widget);
    
    QPalette palette;
    QColor color(255, 255, 255);
    
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
    
    racesdialog_tabs->addTab(tab_widget.main_widget, "+");
    
    racesdialogtab_widgets.push_back(tab_widget);
    
    QObject::connect(tab_widget.validateButton, SIGNAL(clicked(bool)), this, SLOT(sendCreateRace()));
    QObject::connect(tab_widget.colorButton, SIGNAL(clicked(bool)), tab_widget.ColorDiag, SLOT(open()));
    QObject::connect(tab_widget.ColorDiag, SIGNAL(colorSelected(QColor)), this, SLOT(update_colors()));
    std::cout << "oui" << std::endl;
    sendRacesParam();
    std::cout << "non" << std::endl;
}

void MainWindow::update_colors(){
    std::cout << "yah" << std::endl;
    
    for (std::size_t i = 0; i < racesdialogtab_widgets.size(); i++){
        QPalette palette;
        palette.setColor(QPalette::Button, racesdialogtab_widgets[i].ColorDiag->currentColor());
        racesdialogtab_widgets[i].colorButton->setPalette(palette);
    }
}


