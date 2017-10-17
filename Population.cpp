#include "Population.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <cstdlib>
#include <sstream>
#include <QApplication>

#ifndef toSTR
#define toSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
#endif //toSTR

#include <QApplication>

Population::Population(const int &nb_angles, const int &nb_frames, const sf::Vector2f &start)
{
  this->nb_angles = nb_angles;
  this->nb_frames = nb_frames;

  this->start = start;
  cur_frame = 0;
  best_distever = 0;

  cur_gen = 0;

  font.loadFromFile(std::string(QApplication::applicationDirPath().toStdString()+"/BebasNeue.otf"));
}

void Population::createRace(const int &pc_obey, const int &nb_rockets)
{
  Race race;
  races.push_back(race);
  for (int i = 0; i < nb_rockets; i++){
    races[races.size()-1].rockets.push_back(Rocket(pc_obey, nb_angles, nb_frames));
  }
  
  for (int i = 0; i < nb_angles; i++)races[races.size()-1].cur_bestangles.push_back(0);
  
  races[races.size()-1].raceData.pc_obey = pc_obey;
  races[races.size()-1].raceData.success = 0;
  
  sf::Color c((float)races[races.size()-1].raceData.pc_obey*2.55f, 255-((float)races[races.size()-1].raceData.pc_obey*2.55), 255);
  races[races.size()-1].raceData.color = c;
  
  races[races.size()-1].raceData.nb_rockets = races[races.size()-1].rockets.size();
  
  
 races[races.size()-1].cur_supportedecart = 1000;
}

void Population::createCible(const sf::Vector2f &pos){
  pos_cible = pos;
}

void Population::drawPopulation(sf::RenderTarget &target)
{
  sf::ConvexShape shape(3);
  shape.setPoint(0, sf::Vector2f(0, 0));
  shape.setPoint(1, sf::Vector2f(6, 0));
  shape.setPoint(2, sf::Vector2f(3, 10));
  for (std::size_t r = 0; r < races.size(); r++)
  {
    shape.setFillColor(races[r].raceData.color);
    for (int i = 0; i < races[r].raceData.nb_rockets; i++)
    {
      shape.setOrigin(shape.getGlobalBounds().width/2, shape.getGlobalBounds().height/2);
      shape.setRotation(races[r].rockets[i].getAngle()-90);
      shape.setPosition(start.x+races[r].rockets[i].getPos().x, start.y+races[r].rockets[i].getPos().y);
      target.draw(shape);
    }
  }

  sf::RectangleShape rectShape;
  rectShape.setFillColor(sf::Color::White);
  for (std::size_t i = 0; i < rectObs.size(); i++){
    rectShape.setSize(sf::Vector2f(rectObs[i].rect.width, rectObs[i].rect.height));
    rectShape.setPosition(rectObs[i].rect.left, rectObs[i].rect.top);
    target.draw(rectShape);
    if (!rectObs[i].fixed){
        sf::CircleShape pt(4);
        pt.setOrigin(pt.getGlobalBounds().width/2, pt.getGlobalBounds().height/2);
        pt.setFillColor(sf::Color::Yellow);

        //LEFT TOP
        pt.setPosition(rectObs[i].rect.left, rectObs[i].rect.top);
        target.draw(pt);

        //RIGHT TOP
        pt.setPosition(rectObs[i].rect.left+rectObs[i].rect.width, rectObs[i].rect.top);
        target.draw(pt);

        //RIGHT DOWN
        pt.setPosition(rectObs[i].rect.left+rectObs[i].rect.width, rectObs[i].rect.top+rectObs[i].rect.height);
        target.draw(pt);

        //LEFT DOWN
        pt.setPosition(rectObs[i].rect.left, rectObs[i].rect.top+rectObs[i].rect.height);
        target.draw(pt);
    }
  }

  sf::CircleShape cible(10);
  cible.setOrigin(cible.getGlobalBounds().width/2, cible.getGlobalBounds().height/2);
  cible.setPosition(pos_cible);
  cible.setFillColor(sf::Color::White);
  target.draw(cible);

  //STATS
  sf::RectangleShape limit(sf::Vector2f(5, 480));
  limit.setPosition(640-2, 0);
  target.draw(limit);

  std::ostringstream stream;

  sf::Text genText;
  genText.setFont(font);
  stream << cur_gen;
  genText.setString(std::string("current generation : "+stream.str()));
  genText.setCharacterSize(20);
  genText.setPosition(840-10-genText.getGlobalBounds().width, 10);
  genText.setFillColor(sf::Color::White);
  target.draw(genText);

  sf::Text dataText;
  dataText.setFont(font);
  dataText.setString("% of success : ");
  dataText.setCharacterSize(20);
  dataText.setPosition(840-50-dataText.getGlobalBounds().width, 50);
  dataText.setFillColor(sf::Color::White);
  target.draw(dataText);

  sf::Vector2f pos = {700, 100};
  for (std::size_t r = 0; r < races.size(); r++){
    sf::Text txt;
    txt.setFont(font);
    txt.setString(std::string("breed "+toSTR(r)+" : "+toSTR(races[r].raceData.success)));
    txt.setCharacterSize(20);
    txt.setPosition(pos);
    txt.setFillColor(races[r].raceData.color);
    target.draw(txt);
    pos.y += 40;
  }

}

void Population::movePopulation(){
  for (std::size_t r = 0; r < races.size(); r++)
  {
    for (int i = 0; i < races[r].raceData.nb_rockets; i++)
    {
      sf::Vector2f pos = races[r].rockets[i].getPos();
      for (std::size_t o = 0; o < rectObs.size(); o++){
        if (rectObs[o].rect.contains(pos.x+start.x, pos.y+start.y)){
          races[r].rockets[i].stop();
          break;
        }
        else if (pos.x+start.x <= 0 || pos.x+start.x >= 640 || pos.y+start.y <= 0 || pos.y+start.y > 480){
          races[r].rockets[i].stop();
          break;
        }
      }

      races[r].rockets[i].move();
    }
  }
  if (cur_frame>=nb_frames){
      restartPopulation();
      cur_frame = 0;
      cur_gen++;
      std::cout << "new gen = " << cur_gen << std::endl;
  }
  else{
    cur_frame++;
  }
}

void Population::resetPopulation(){
    cur_frame = 0;
    cur_gen = 0;
    for (std::size_t r = 0; r < races.size(); r++){
        for (int i = 0; i < races[r].raceData.nb_rockets; i++){
            races[r].rockets[i].restart();
        }
    }
}

void Population::restartGeneration(){
    for (std::size_t r = 0; r < races.size(); r++){
        for (int i = 0; i < races[r].raceData.nb_rockets; i++){
            std::cout << "yeah 1 : r = " << r << std::endl;
            std::cout << "stat : " << races[r].cur_supportedecart << ", " << races[r].cur_bestangles.size() << std::endl;
            races[r].rockets[i].restartFrom(races[r].cur_bestangles, races[r].cur_supportedecart);
            std::cout << "yeah 2" << std::endl;
        }
    }
}

void Population::restartPopulation(){
  float best_dist = 0;int indexbest_dist = 0;
  for (std::size_t r = 0; r < races.size(); r++)
  {
    best_dist = 0;
    indexbest_dist = 0;
    int dists = 0;
    for (int i = 0; i < races[r].raceData.nb_rockets; i++)
    {
      sf::Vector2f pos = {races[r].rockets[i].getPos().x+start.x, races[r].rockets[i].getPos().y+start.y};
      float dist = std::sqrt(std::pow((pos_cible.x-pos.x), 2)+std::pow((pos_cible.y-pos.y), 2));
      dists+=dist;
      if (i == 0 || dist<best_dist){
        best_dist = dist;
        indexbest_dist = i;
      }
    }
    dists /= races[r].raceData.nb_rockets;
    int ecart = (best_dist*3000)/600;
    races[r].cur_supportedecart = ecart;
    races[r].raceData.success = 100-(dists*100/600);
    
    races[r].cur_bestangles.clear();
    for (std::size_t a = 0; a < races[r].rockets[indexbest_dist].getAngles()->size(); a++){
        races[r].cur_bestangles.push_back((*(races[r].rockets[indexbest_dist].getAngles()))[a]);
    }
    for (int i = 0; i < races[r].raceData.nb_rockets; i++){
      if (best_dist<1){
        races[r].rockets[i].restartFrom((*(races[r].rockets[indexbest_dist].getAngles())), 0);
      }else{
        races[r].rockets[i].restartFrom((*(races[r].rockets[indexbest_dist].getAngles())), ecart);
      }
    }
  }
}

void Population::createRectobs(const sf::FloatRect &rect, bool fixed){
    RectObs tmp = {rect, fixed, {false, false, false, false}};
    rectObs.push_back(tmp);
}

int Population::getCurrentGeneration(){
  return cur_gen;
}

void Population::deleteObstacle(const sf::Vector2f &mousepos){
    for (int i = rectObs.size()-1; i >= 0; i--){
        if (rectObs[i].rect.contains(mousepos)){
            rectObs.erase(rectObs.begin()+i);
            break;
        }
    }
}

void Population::deleteAllObstacles(){
    rectObs.clear();
}

void Population::setObsUnfixed(const sf::Vector2f &mousepos){
    for (int i = rectObs.size()-1; i >= 0; i--){
        if (rectObs[i].rect.contains(mousepos)){
            rectObs[i].fixed = false;
        }
    }
}

void Population::setObsFixed(const sf::Vector2f &mousepos){
    for (int i = rectObs.size()-1; i >= 0; i--){
        if (!rectObs[i].rect.contains(mousepos)){
            rectObs[i].fixed = true;
        }
    }
}

bool Population::obstacleUnfixed(){
    for (int i = rectObs.size()-1; i >= 0; i--){
        if (!rectObs[i].fixed)return true;
    }
    return false;
}

void Population::setMoveObs(const sf::Vector2f &mousepos){
    int marge = 6;
    bool changed = false;
    for (int i = rectObs.size()-1; i >= 0; i--){
        sf::FloatRect rect = rectObs[i].rect;
        rectObs[i].moves[0] = false;
        rectObs[i].moves[1] = false;
        rectObs[i].moves[2] = false;
        rectObs[i].moves[3] = false;
        if (!rectObs[i].fixed){
            if (mousepos.y <= rect.top+rect.height && mousepos.y >= rect.top){
                if (mousepos.x <= rect.left+marge/2 && mousepos.x >= rect.left-marge/2){
                    QApplication::setOverrideCursor(Qt::SizeHorCursor);
                    changed = true;
                    rectObs[i].moves[LEFT] = true;
                }
                if (mousepos.x <= rect.left+rect.width+marge/2 && mousepos.x >= rect.left+rect.width-marge/2){
                    QApplication::setOverrideCursor(Qt::SizeHorCursor);
                    changed = true;
                    rectObs[i].moves[RIGHT] = true;
                }
            }
            
            if (mousepos.x <= rect.left+rect.width && mousepos.x >= rect.left){
                if (mousepos.y <= rect.top+marge/2 && mousepos.y >= rect.top-marge/2){
                    QApplication::setOverrideCursor(Qt::SizeVerCursor);
                    changed = true;
                    rectObs[i].moves[TOP] = true;
                }
                if (mousepos.y <= rect.top+rect.height+marge/2 && mousepos.y >= rect.top+rect.height-marge/2){
                    QApplication::setOverrideCursor(Qt::SizeVerCursor);
                    changed = true;
                    rectObs[i].moves[DOWN] = true;
                }
            }
        }
    }
    if (!changed){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void Population::applyObsMoves(const sf::Vector2f &mousepos){
    int marge = 10;
    for (int i = rectObs.size()-1; i >= 0; i--){
        if (!rectObs[i].fixed){
            if (rectObs[i].moves[RIGHT]){
                rectObs[i].rect.width = mousepos.x-rectObs[i].rect.left;
                if (rectObs[i].rect.width < marge)rectObs[i].rect.width = marge;
            }
            
            else if (rectObs[i].moves[DOWN]){
                rectObs[i].rect.height = mousepos.y-rectObs[i].rect.top;
                if (rectObs[i].rect.height < marge)rectObs[i].rect.height = marge;
            }
            
            else if (rectObs[i].moves[LEFT]){
                float ecart = rectObs[i].rect.left-mousepos.x;
                float lpos = rectObs[i].rect.left+rectObs[i].rect.width;
                rectObs[i].rect.left = mousepos.x;
                rectObs[i].rect.width += ecart;
                if (rectObs[i].rect.width < marge){
                    rectObs[i].rect.width = marge;
                    rectObs[i].rect.left = lpos-marge;
                }
            }
            
            else if (rectObs[i].moves[TOP]){
                float ecart = rectObs[i].rect.top-mousepos.y;
                float lpos = rectObs[i].rect.top+rectObs[i].rect.height;
                rectObs[i].rect.top = mousepos.y;
                rectObs[i].rect.height += ecart;
                if (rectObs[i].rect.height < marge){
                    rectObs[i].rect.height = marge;
                    rectObs[i].rect.top = lpos-marge;
                }
            }
            else{
                rectObs[i].rect.left = mousepos.x-rectObs[i].rect.width/2;
                rectObs[i].rect.top = mousepos.y-rectObs[i].rect.height/2;
            }
        }
    } 
}

std::vector<Race> *Population::getRaces(){
    std::cout << "races size : " << races.size() << std::endl;
    return &races;
}
void Population::setRaces(const std::vector<RaceData> &races){
    std::cout << "oui" << std::endl;
    while (races.size() > this->races.size()){
        createRace(50, 0);
    }
    while (races.size() < this->races.size()){
        this->races.erase(this->races.begin());
    }
    for (std::size_t i = 0; i < races.size(); i++){
        std::cout << "fds 1" << std::endl;
        this->races[i].cur_bestangles;
        this->races[i].raceData = races[i];
         std::cout << "fds 2 " << this->races[i].raceData.nb_rockets << " | " << this->races[i].rockets.size() << " i = " << i << std::endl;
        while ((std::size_t)races[i].nb_rockets > this->races[i].rockets.size()){
            this->races[i].rockets.push_back(Rocket(Rocket(races[i].pc_obey, nb_angles, nb_frames)));//, this->races[i].cur_bestangles, this->races[i].cur_supportedecart)));
        }
        std::cout << "fds 3" << std::endl;
        while ((std::size_t)races[i].nb_rockets < this->races[i].rockets.size()){
            this->races[i].rockets.erase(this->races[i].rockets.begin());
        }
        std::cout << "fds 4" << std::endl;
    }
    std::cout << "non" << std::endl;
}
