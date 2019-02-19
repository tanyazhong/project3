#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject{
public: 
	Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth, double size);
	virtual void doSomething() = 0;
	//virtual void getBurned() = 0;
	StudentWorld* getWorld() const;
	bool alive() const;
	bool blocker() const;
	bool actorCanMove(int dest_x, int dest_y) const;
private:
	StudentWorld* m_world;
	bool m_alive = true;
	bool m_blocker = true;
};

class Goodie : public Actor {        //vaccine, landmine goodie, gas can
	
};

class Human : public Actor {       //penelope, citizns
public:
	Human(StudentWorld* sw, int imageID, int x, int y);
	void getInfected();
	bool infected() const;
	int infections() const;
private:
	bool m_infected = false;
	int m_nInfections = 0;
};

class Penelope : public Human {
public:
	Penelope(StudentWorld * sw, int x, int y);
	virtual void doSomething();
	
private:
	int m_nLandmines = 0;
	int m_nFlamethrowers = 0;
	int m_nVaccines = 0;
};

class Wall : public Actor {
public:
	Wall(StudentWorld* sw, int x, int y);
	virtual void doSomething();
};



#endif // ACTOR_H_
