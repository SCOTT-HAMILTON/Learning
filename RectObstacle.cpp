#include "RectObstacle.h"

RectObstacle::RectObstacle(const sf::FloatRect &rect){
    this->rect = rect;
}

RectObstacle::RectObstacle()
{
    rect.width = 0;
    rect.height = 0;
    rect.left = 0;
    rect.top = 0;
}

bool touchRocket(const sf::Vector2f &pos){
 return 0;
}

void draw(const sf::RenderTarget &target){

}
