#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <iostream>
#include "QSFMLCanvas.h"
#include <QMouseEvent>

class MyCanvas : public QSFMLCanvas
{
    Q_OBJECT
public:
    MyCanvas(QSize windowSize, QWidget *parent = NULL);
    virtual ~MyCanvas();
    int getCurrentGeneration();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void gotoGen(int);
    void resetGen();
    void moveCible();
    void addRectObst();

signals:
    void genChanged(int);

private:
    bool update_pop;
    Population pop;
    virtual void OnInit();

    virtual void OnUpdate();

    void waitPopTo();
    int genGoto;
    int genStart;
    bool drawProgBar;
    bool move_cible;

    bool Controlpressed;
    bool Deletepressed;

    bool leftMousePressed;
};

#endif // MYCANVAS_H
