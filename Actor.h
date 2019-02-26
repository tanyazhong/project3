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

class Exit : public Actor{
public:
	Exit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual bool blocksFlame() const;
};

class Pit : public Actor{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
};

class Flame : public Actor{
public:
	Flame(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_lifeSpan = 0;
};

class Vomit : public Actor{
public:
	Vomit(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
private:
	int m_lifeSpan = 0;
};

class Landmine : public Actor{
public:
	Landmine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void activateIfAppropriate(Actor* a);
	virtual void dieByFallOrBurnIfAppropriate();
};

class Goodie : public Actor{
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
	void beVomitedOnIfAppropriate();
	virtual bool blocksMovement() const;
	bool isInfected() const;
	int infectionDuration() const;
	void clearInfection();
	void increaseInfections();
private:
	int m_nInfections = 0;
};

class Penelope : public Human {
public:
	Penelope(StudentWorld * sw, double x, double y);
	virtual void doSomething();
	virtual void useExitIfAppropriate();
	virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);
	void increaseVaccines();
	void increaseFlameCharges();
	void increaseLandmines();
	int getNumVaccines() const;
	int getNumFlameCharges() const;
	int getNumLandmines() const;
	
private:
	void movePenelope(Direction d, double x, double y);
	void deployFlames(Direction d, double x, double y);
	void deployLandmine(Direction d, double x, double y);
	void vaccinate();
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
