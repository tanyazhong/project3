#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

class Actor : public GraphObject{
public: 
	Actor(StudentWorld* sw, int imageID, double startX, double startY, Direction dir, int depth);
	virtual void doSomething() = 0;
	StudentWorld* getWorld() const;
	bool isAlive() const;
	void setDead();
	virtual bool blocksMovement() const;       //automatically false
	virtual bool blocksFlame() const;          //auto false
	virtual void activateIfAppropriate(Actor* a);
	virtual void useExitIfAppropriate();
	bool actorCanMove(double dest_x, double dest_y) const;
	virtual ~Actor();
private:
	StudentWorld* m_world;
	bool m_alive = true;
};

class Wall : public Actor {
public:
	Wall(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual bool blocksFlame() const;
};

class ActivatingObject : public Actor
{
public:
	ActivatingObject(StudentWorld* sw, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject{
public:
	Exit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Goodie : public Actor {        //vaccine, landmine goodie, gas can
	
};

class Zombie : public Actor {

};

class dumbZombie : public Zombie {

};

class smartZombie : public Zombie {

};

class Human : public Actor {       //penelope, citizns
public:
	Human(StudentWorld* sw, int imageID, double x, double y);
	void getInfected();
	virtual bool blocksMovement() const;
	bool isInfected() const;
	int infections() const;
private:
	bool m_infected = false;
	int m_nInfections = 0;
};

class Penelope : public Human {
public:
	Penelope(StudentWorld * sw, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	
private:
	void movePenelope(Direction d, double x, double y);
	int m_nLandmines = 0;
	int m_nFlamethrowers = 0;
	int m_nVaccines = 0;
};

class Citizen : public Human {
	//virtual void useExitIfAppropriate();
};



#endif // ACTOR_H_
