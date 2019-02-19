#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <vector>
#include <string>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	virtual ~StudentWorld();
	bool canMove(double x, double y) const;
private:
	std::vector<Actor*> m_actors;
	Penelope* m_pen;
};


#endif // STUDENTWORLD_H_
