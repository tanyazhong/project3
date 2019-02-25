#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
class Goodie;
class Penelope;

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
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void beVomitedOnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie * g);
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

class ActivatingObject : public Actor{
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

class Pit : public ActivatingObject{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject{
public:
	Flame(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Vomit : public ActivatingObject{
public:
	Vomit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Landmine : public ActivatingObject{
public:
	Landmine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
};

class Goodie : public ActivatingObject{
public:
	Goodie(StudentWorld* sw, int imageID, double x, double y);
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUp(Penelope* p) = 0; // Have p pick up this goodie.
};

class VaccineGoodie : public Goodie{
public:
	VaccineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie {
public:
	GasCanGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie {
public:
	LandmineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickUp(Penelope* p);
};

class Agent : public Actor{
public:
	Agent(StudentWorld* w, int imageID, double x, double y, int dir);
	virtual bool blocksMovement() const;
	virtual bool triggersOnlyActiveLandmines() const;
};


class Zombie : public Agent{
public:
	Zombie(StudentWorld* sw, double x, double y);
	
};

class DumbZombie : public Zombie{
public:
	DumbZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
};

class SmartZombie : public Zombie
{
public:
	SmartZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void dieByFallOrBurnIfAppropriate();
};

class Human : public Agent {       //penelope, citizns
public:
	Human(StudentWorld* sw, int imageID, double x, double y);
	void getVomitedOn();
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
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);
	
private:
	void movePenelope(Direction d, double x, double y);
	int m_nLandmines = 0;
	int m_nFlamethrowers = 0;
	int m_nVaccines = 0;
};

class Citizen : public Human
{
public:
	Citizen(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
};


#endif // ACTOR_H_
