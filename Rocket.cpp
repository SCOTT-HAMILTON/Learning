#include "Rocket.h"
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI
#endif//M_PI

const int marge = 10;

Rocket::Rocket(const int &obey, const int &nb_angles, const int &nb_frames)
{
  this->pc_obey = obey;
  this->nb_angles = nb_angles;
  this->nb_frames = nb_frames;
  int angle = std::rand()%180-180;
  for (auto i = 0; i < nb_angles; i++){
    if (rand()%1000>500){
      angle+=rand()%marge;
    }else{
      angle-=rand()%marge;
    }
    angles.push_back(angle);
  }
  cur_frame = 0;
  pos = {0, 0};
  can_move = true;
}

void Rocket::move(){
  const int fps = 60;
  const int speed = 100;
  if (cur_frame<nb_frames-1 && can_move){
    float rad = (float)M_PI*angles[cur_frame]/180;

    pos.x += std::cos(rad)*speed/fps;
    pos.y += std::sin(rad)*speed/fps;
    if (cur_frame+1<nb_frames)cur_frame++;
  }
}

sf::Vector2f Rocket::getPos(){
  return pos;
}

int Rocket::getAngle(){
  return angles[cur_frame];
}

std::vector<int> *Rocket::getAngles(){
  return &angles;
}

void Rocket::restart(){
  cur_frame = 0;
  pos = {0, 0};
  can_move = true;
  int angle = std::rand()%180-180;
  for (auto i = 0; i < nb_angles; i++){
    if (rand()%1000>500){
      angle+=rand()%marge;
    }else{
      angle-=rand()%marge;
    }
    angles[i] = angle;
  }
}

void Rocket::restartFrom(std::vector<int> angles, const int &supported_ecart){
  cur_frame = 0;
  pos = {0, 0};
  can_move = true;
  int angle = std::rand()%180-180;
  int ecart = 0;
  for (auto i = 0; i < nb_angles; i++){
    if (supported_ecart == 0){
      angle = angles[i];
    }
    else if (rand()%100<pc_obey){
      angle += (angles[i]-angle)%marge;
      if (ecart != 0){
        ecart -= (ecart/std::abs(ecart))*supported_ecart;
      }

    }else if (std::abs(ecart)>=supported_ecart){
      angle += ecart%marge;
    }
    else{
      if (rand()%1000>500){
        angle+=rand()%marge;
      }else{
        angle-=rand()%marge;
      }
    }
    if ((angles[i]-angle) != 0){
      ecart += (angles[i]-angle);
    }
    this->angles[i] = angle;
  }
}

void Rocket::stop(){
  can_move = false;
}
