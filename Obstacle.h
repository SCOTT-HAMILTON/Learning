#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Obstacle
{

public:
    Obstacle();
    virtual bool touchRocket(const sf::Vector2f &pos);
    virtual void draw(const sf::RenderTarget &target);
};

#endif // OBSTACLE_H
