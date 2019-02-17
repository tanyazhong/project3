#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject{
public: 
	Actor(int imageID, double startX, double startY, Direction dir, int depth, double size);
	virtual void doSomething() = 0;
	virtual void getBurned() = 0;

private:
	
};

class Goodie : public Actor {        //vaccine, landmine goodie, gas can
	bool m_alive;
};

class Human : public Actor {       //penelope, citizns, zombs
public:
	Human(int imageID, int x, int y);
	void getInfected();
private:
	bool m_alive;
	bool m_infected;
	int m_nInfections;
};

class Penelope : public Human {
public:
	Penelope(int x, int y);
	virtual void doSomething();
	StudentWorld* getWorld() const;
private:
	StudentWorld* m_world;

	
};

class Wall : public Actor {
public:
	Wall(int x, int y);
	virtual void doSomething();
};



#endif // ACTOR_H_
