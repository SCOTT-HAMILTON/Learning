#ifndef ROCKET_H
#define ROCKET_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class Rocket
{
public:
  Rocket(const int &obey, const int &nb_angles, const int &nb_frames);
  Rocket(const int &obey, const int &nb_angles, const int &nb_frames, std::vector<int> angles, const int &supported_ecart = 3000);
  void move();
  sf::Vector2f getPos();
  int getAngle();
  void restart();
  void restartFrom(std::vector<int> angles, const int &supported_ecart = 3000);
  std::vector<int> *getAngles();
  void stop();

private:
  int pc_obey;
  std::vector<int> angles;
  int nb_angles;
  int nb_frames;
  sf::Vector2f pos;
  int cur_frame;
  bool can_move;
};

#endif //ROCKET_H
