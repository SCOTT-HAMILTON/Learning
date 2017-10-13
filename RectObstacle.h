#ifndef RECTOBSTACLE_H
#define RECTOBSTACLE_H

#include <iostream>
#include "Obstacle.h"

class RectObstacle : public Obstacle
{
public:
    RectObstacle(const sf::FloatRect &rect);
    RectObstacle();
    virtual bool touchRocket(const sf::Vector2f &pos);
    virtual void draw(const sf::RenderTarget &target);

private:
    sf::FloatRect rect;

};

#endif // RECTOBSTACLE_H
