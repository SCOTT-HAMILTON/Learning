#include "MainWindow.h"

MainWindow::MainWindow()
{
    setFixedSize({1065, 480});
    mainLay = new QHBoxLayout;

    canvas = new MyCanvas({840, 480});
    canvas->setFixedSize({840, 480});

    controlLay = new QVBoxLayout;

    genTxtLabel = new QLabel("go to a higher generation : ");

    genLay = new  QVBoxLayout;

    genBox = new QSpinBox;
    genBox->setValue(canvas->getCurrentGeneration());
    std::cout <<"val : " << canvas->getCurrentGeneration() << std::endl;
    genBox->setMaximum(genBox->value()+10000);
    genBox->setMinimum(genBox->value());

    validGenButton = new QPushButton("go to this generation");
    restartGenButton = new QPushButton("reset generation");

    genLay->addWidget(genTxtLabel);
    genLay->addWidget(genBox);
    genLay->addWidget(validGenButton);
    genLay->addWidget(restartGenButton);

    box_gen = new QGroupBox("generations");
    box_gen->setLayout(genLay);

    moveCibleText = new QPushButton("change cible position");

    createObstacle = new QPushButton("create rectangle obstacle");

    controlLay->addWidget(box_gen, 100, Qt::AlignTop|Qt::AlignLeft);
    controlLay->addWidget(moveCibleText, 30, Qt::AlignCenter|Qt::AlignLeft);
    controlLay->addWidget(createObstacle, 100, Qt::AlignTop|Qt::AlignLeft);
    mainLay->addLayout(controlLay);
    mainLay->addWidget(canvas);

    setLayout(mainLay);

    QObject::connect(canvas, SIGNAL(genChanged(int)), this, SLOT(changeGen(int)));
    QObject::connect(validGenButton, SIGNAL(clicked(bool)), this, SLOT(sendGotoGen()));
    QObject::connect(restartGenButton, SIGNAL(clicked(bool)), this, SLOT(sendRestartGen()));
    QObject::connect(moveCibleText, SIGNAL(clicked(bool)), this, SLOT(sendMoveCible()));
    QObject::connect(createObstacle, SIGNAL(clicked(bool)), canvas, SLOT(addRectObst()));
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


