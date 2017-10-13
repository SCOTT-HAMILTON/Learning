#include "MyCanvas.h"
#include <memory>
#include <QApplication>

std::unique_ptr<sf::Thread> threadPopWaiter;

MyCanvas::MyCanvas(QSize windowSize, QWidget *parent) :
    QSFMLCanvas({0,0}, windowSize, 0, parent), update_pop(true),
    drawProgBar(false), move_cible(false), leftMousePressed(false)
{
    threadPopWaiter = std::unique_ptr<sf::Thread> (new sf::Thread(&MyCanvas::waitPopTo, this));
    qApp->installEventFilter(this);
}

MyCanvas::~MyCanvas(){
    genGoto = 0;
    (threadPopWaiter.get())->wait();
}

void MyCanvas::OnInit(){
    pop = Population(400, 400, sf::Vector2f(320, 480));
    pop.createRace(90, 200);
    pop.createRace(70, 200);
    pop.createRace(50, 200);

    pop.createCible(sf::Vector2f(320, 100));
    pop.createRectobs(sf::FloatRect(0, 230, 500, 20));



    setFramerateLimit(200);
}

void MyCanvas::OnUpdate(){
    clear();

    if (move_cible){
        QPoint pos =  this->mapFromGlobal(QCursor::pos());
        pop.createCible(sf::Vector2f(pos.x(), pos.y()));
    }
    if (update_pop){
        int gen = pop.getCurrentGeneration();
        pop.movePopulation();
        if (gen != pop.getCurrentGeneration())emit(genChanged(pop.getCurrentGeneration()));
        pop.drawPopulation((*(this)));

    }
    if (drawProgBar){
        int w = (pop.getCurrentGeneration()-genStart)*300/(genGoto-genStart);
        sf::RectangleShape progBar(sf::Vector2f(w, 32));
        progBar.setPosition(getSize().x/2-150, getSize().y/2-progBar.getGlobalBounds().height/2-1);
        progBar.setFillColor(sf::Color(8, 204, 0));

        sf::RectangleShape outlines(sf::Vector2f(300, 30));
        outlines.setOutlineThickness(2);
        outlines.setOutlineColor(sf::Color::White);
        outlines.setFillColor(sf::Color::Transparent);
        outlines.setPosition(getSize().x/2-150, getSize().y/2-outlines.getGlobalBounds().height/2);
        draw(progBar);
        draw(outlines);
    }
    display();
}

int MyCanvas::getCurrentGeneration(){
    return pop.getCurrentGeneration();
}

void MyCanvas::gotoGen(int val){
    genGoto = 0;
    (threadPopWaiter.get())->wait();
    update_pop = false;
    genGoto = val;
    genStart = getCurrentGeneration();
    drawProgBar = true;
    (threadPopWaiter.get())->launch();
}

void MyCanvas::resetGen(){
    genGoto = 0;
    (threadPopWaiter.get())->wait();
    pop.resetPopulation();
}

void MyCanvas::moveCible(){
    move_cible = true;
}

void MyCanvas::waitPopTo(){
    while (pop.getCurrentGeneration()<genGoto){
        pop.movePopulation();
    }
    update_pop = true;
    drawProgBar = false;

    std::cout << "finished!" << std::endl;
}

void MyCanvas::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        leftMousePressed = true;
        move_cible = false;
        QPoint pos =  this->mapFromGlobal(QCursor::pos());
        pop.setObsFixed(sf::Vector2f(pos.x(), pos.y()));
        if (Deletepressed){
            pop.deleteObstacle(sf::Vector2f(pos.x(), pos.y()));
        }else if (Controlpressed){
            pop.setObsUnfixed(sf::Vector2f(pos.x(), pos.y()));
        }
    }
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        leftMousePressed = false;
    }
}

bool MyCanvas::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Control){
            Controlpressed = true;
        }
        if (keyEvent->key() == Qt::Key_Delete){
            Deletepressed = true;
        }
    }

    else if (event->type() == QEvent::KeyRelease){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Control){
            Controlpressed = false;
        }
        if (keyEvent->key() == Qt::Key_Delete){
            Deletepressed = false;
        }
    }

    else if (event->type() == QEvent::MouseMove){
        QPoint pos =  this->mapFromGlobal(QCursor::pos());
        if (leftMousePressed){
            pop.applyObsMoves(sf::Vector2f(pos.x(), pos.y()));
        }else{
            pop.setMoveObs(sf::Vector2f(pos.x(), pos.y()));
        }
    }

    return QObject::eventFilter(obj, event);
}

void MyCanvas::addRectObst(){
    sf::FloatRect rect;
    rect.width = 50;
    rect.height = 50;
    rect.left = 320-rect.width/2;
    rect.top = 240-rect.height/2;
    pop.createRectobs(rect, false);
}

