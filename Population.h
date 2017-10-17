#ifndef POPUATION_H
#define POPUATION_H

#include <iostream>
#include "Rocket.h"
#include <vector>
#include <SFML/Graphics.hpp>

struct RaceData2{
    int success;
    sf::Color color;
    int pc_obey;
    int nb_rockets;
};

typedef RaceData2 RaceData;

struct Race2{
  std::vector<Rocket> rockets; 
  RaceData raceData;
  std::vector<int> cur_bestangles;
  int cur_supportedecart;
};

typedef Race2 Race;

enum {RIGHT, DOWN, LEFT, TOP};

struct RectObs2{
    sf::FloatRect rect;
    bool fixed;
    bool moves[4];
};

typedef RectObs2 RectObs;

class Population
{
public:
  Population(const int &nb_angles=0, const int &nb_frames=0, const sf::Vector2f &start = {0, 0});
  void createRace(const int &pc_obey, const int &nb_rockets);
  void createCible(const sf::Vector2f &pos);
  void drawPopulation(sf::RenderTarget &target);
  void movePopulation();
  void createRectobs(const sf::FloatRect &rect, bool fixed = true);
  void resetPopulation();
  void restartGeneration();
  int getCurrentGeneration();
  void deleteObstacle(const sf::Vector2f &mousepos);
  void deleteAllObstacles();
  void setObsUnfixed(const sf::Vector2f &mousepos);
  void setObsFixed(const sf::Vector2f &mousepos);
  bool obstacleUnfixed();
  void setMoveObs(const sf::Vector2f &mousepos);
  void applyObsMoves(const sf::Vector2f &mousepos);
  std::vector<Race> *getRaces();
  void setRaces(const std::vector<RaceData> &races);

private:
  int nb_angles;
  int nb_frames;
  int cur_frame;
  float best_distever;
  std::vector<Race> races;
  sf::Vector2f start;
  sf::Vector2f pos_cible;
  void restartPopulation();
  std::vector<RectObs> rectObs;
  int cur_gen;
  sf::Font font;
};

#endif //POPUATION_H
