#ifndef QSFMLCANVAS_H
#define QSFMLCANVAS_H

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include "Population.h"

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :
    QSFMLCanvas(const QPoint& Position = {0, 0}, const QSize& Size = {640, 480}, unsigned int FrameTime = 0, QWidget* Parent = NULL);

    virtual ~QSFMLCanvas();

private :

    virtual void OnInit(){}

    virtual void OnUpdate(){}

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    QTimer myTimer;
    bool   myInitialized;


};

#endif // QSMLCANVAS_H
